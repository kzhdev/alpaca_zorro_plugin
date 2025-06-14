// The MIT License (MIT) 
// Copyright (c) 2024-2025 Kun Zhao
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <tlhelp32.h>
#endif

#include <cstdint>
#include <thread>
#include <atomic>
#include <unordered_set>
#include <functional>
#include <filesystem>
#include <format>

#include <websocket_proxy\types.h>

namespace websocket_proxy {

class WebsocketProxyCallback {
public:
    virtual ~WebsocketProxyCallback() = default;
    virtual void onWebsocketProxyServerDisconnected() = 0;
    virtual void onWebsocketOpened(uint64_t id) = 0;
    virtual void onWebsocketClosed(uint64_t id) = 0;
    virtual void onWebsocketError(uint64_t id, const char* err, uint32_t len) = 0;
    virtual void onWebsocketData(uint64_t id, const char* data, uint32_t len, uint32_t remaining) = 0;

    // functions to pass log messages to client
    virtual void logError(std::function<std::string()>&&) {}
    virtual void logWarning(std::function<std::string()>&&) {}
    virtual void logInfo(std::function<std::string()>&&) {}
    virtual void logDebug(std::function<std::string()>&&) {}
};

class WebsocketProxyClient {
public:
    WebsocketProxyClient(WebsocketProxyCallback* callback, std::string&& name, std::string &&proxy_exe_path);
    virtual ~WebsocketProxyClient();

    uint64_t serverId() const noexcept { return server_pid_; }

    std::pair<uint64_t, bool> openWebSocket(const std::string& url, const std::string &api_key);
    bool openWebSocketAsync(const std::string& url, const std::string &api_key);
    bool closeWebSocket(uint64_t id = 0);
    bool subscribe(uint64_t id, const std::string& symbol, const char* subscription_request, uint32_t request_len, SubscriptionType type, bool& existing);
    bool unsubscribe(uint64_t id, const std::string& symbol, const char* unsubscription_request, uint32_t request_len);
    void send(uint64_t id, const char* msg, uint32_t len);

private:
    bool connect();
    bool spawnWebsocketsProxyServer();
    void waitForServerReady();
    bool _register();
    void unregister();
    void sendMessage(Message* msg, uint64_t index, uint32_t size);
    Message* sendOpenMessage(const std::string& url, const std::string &api_key);
    bool waitForResponse(Message* msg, uint32_t timeout = 10000);
    bool sendHeartbeat(uint64_t now);
    void doWork();
    void handleWsOpen(Message* msg);
    void handleWsClose(Message* msg);
    void handleWsError(Message* msg);
    void handleWsData(Message* msg);

    template<typename T>
    std::tuple<Message*, uint64_t, uint32_t> reserveMessage(uint32_t data_size = 0);

    std::tuple<Message*, uint64_t, uint32_t> reserveMessage();
    
private:
    WebsocketProxyCallback* callback_ = nullptr;
    std::unique_ptr<SHM_QUEUE_T> client_queue_;
    std::unique_ptr<SHM_QUEUE_T> server_queue_;
    uint64_t server_queue_index_ = 0;
    uint64_t last_heartbeat_time_ = 0;
    uint64_t last_server_heartbeat_time_ = 0;
    uint64_t id_ = 0;
    const uint64_t pid_;
    std::atomic<uint64_t> server_pid_ = 0;
    std::shared_ptr<std::atomic_bool> run_;
    std::string name_;
    std::filesystem::path exe_path_;
    std::unordered_set<uint64_t> websockets_;
    std::unique_ptr<std::thread> worker_thread_;
};


////////////////////////////// Helper functions //////////////////////////////

inline uint64_t get_timestamp() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
}

template<typename T>
inline uint32_t get_message_size(uint32_t data_len = 0) {
    return sizeof(Message) + sizeof(T) + data_len * sizeof(char);
}

inline bool isProcessRunning(const wchar_t* name) {
    bool exists = false;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (Process32First(snapshot, &entry)) {
        while (Process32Next(snapshot, &entry)) {
            if (!_wcsicmp(entry.szExeFile, name)) {
                exists = true;
                break;
            }
        }
    }
    CloseHandle(snapshot);
    return exists;
}

inline bool isProcessRunning(uint64_t processID) {
    if (HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, (DWORD)processID)) {
        DWORD exitCodeOut;
        // GetExitCodeProcess returns zero on failure
        if (GetExitCodeProcess(process, &exitCodeOut)) {
            return exitCodeOut == STILL_ACTIVE;
        }
    }
    return false;
}


////////////////////////////// WebsocketProxyClient Implementation //////////////////////////////

inline WebsocketProxyClient::WebsocketProxyClient(WebsocketProxyCallback* callback, std::string&& name, std::string&& proxy_exe_path)
    : callback_(callback)
    , pid_(GetCurrentProcessId())
    , name_(std::move(name))
    , exe_path_(std::move(proxy_exe_path))
{
    if (!std::filesystem::exists(exe_path_))
    {
        throw std::runtime_error(exe_path_.generic_string() + " not exist");
    }
}

inline WebsocketProxyClient::~WebsocketProxyClient()
{
    if (server_pid_.load(std::memory_order_relaxed))
    {
        unregister();
    }
    if (run_)
    {
        run_->store(false, std::memory_order_release);
    }
    if (worker_thread_ && worker_thread_->joinable())
    {
// #ifdef _WIN32
//         worker_thread_->detach();
// #endif // _WIN32
        worker_thread_->join();
    }
}

inline bool WebsocketProxyClient::connect() {
    if (!worker_thread_) {
        worker_thread_ = std::make_unique<std::thread>([this]() { doWork(); });
    }

    if (!spawnWebsocketsProxyServer()) {
        callback_->logError([]() { return "Failed to spawn websocket proxy"; });
        return false;
    }

    waitForServerReady();
    return _register();
}

inline void WebsocketProxyClient::sendMessage(Message* msg, uint64_t index, uint32_t size) {
    msg->status.store(Message::Status::PENDING, std::memory_order_relaxed);
    client_queue_->publish(index, size);
    last_heartbeat_time_ = get_timestamp();
}

inline bool WebsocketProxyClient::waitForResponse(Message* msg, uint32_t timeout) {
    auto start = get_timestamp();
    while (msg->status.load(std::memory_order_relaxed) == Message::Status::PENDING) {
        auto now = get_timestamp();
        if ((now - start) > timeout) {
            return false;
        }
        if (!sendHeartbeat(now)) {
            std::this_thread::yield();
        }
    }
    return true;
}

inline bool WebsocketProxyClient::spawnWebsocketsProxyServer() {
    if (!isProcessRunning(L"websocket_proxy.exe")) {
        STARTUPINFOW si;
        PROCESS_INFORMATION pi;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        callback_->logInfo([]() { return "Spawn websocket_proxy"; });
        if (!CreateProcessW(exe_path_.c_str(), NULL, NULL, NULL, FALSE, DETACHED_PROCESS, NULL, NULL, &si, &pi)) {
            callback_->logError([](){ return std::format("Failed to launch websocket_proxy. err={}", GetLastError()); });
            return false;
        }
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        // wait for proxy to start up
        auto start = get_timestamp();
        while (!isProcessRunning(L"websocket_proxy.exe") && (get_timestamp() - start) < 10000) {
            std::this_thread::yield();
        }
        callback_->logInfo([]() { return "websocket_proxy started"; });
    }

    uint32_t retry = 5;
    while (retry--) {
        try {
            // Waiting for the last queue in proxy server created
            server_queue_ = std::make_unique<SHM_QUEUE_T>(SERVER_TO_CLIENT_QUEUE);
            break;
        }
        catch (const std::runtime_error&) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    if (!server_queue_) {
        callback_->logError([]() { return "Failed to launch websocket_proxy. server_queue_ not ready"; });
        return false;
    }

    try {
        client_queue_ = std::make_unique<SHM_QUEUE_T>(CLIENT_TO_SERVER_QUEUE);
    }
    catch (const std::runtime_error&) {
        callback_->logError([]() { return "Failed to launch websocket_proxy. client_req_queue not ready"; });
        return false;
    }

    server_queue_index_ = server_queue_->initial_reading_index();
    return true;
}

inline void WebsocketProxyClient::waitForServerReady()
{
    auto start = get_timestamp();
    while ((get_timestamp() - start) < 10000) {
        auto result = server_queue_->read(server_queue_index_);
        if (result.first) {
            break;
        }
    }
}

inline bool WebsocketProxyClient::_register() {
    auto [msg, index, size] = reserveMessage<RegisterMessage>();
    msg->type = Message::Type::Regster;
    auto reg = reinterpret_cast<RegisterMessage*>(msg->data);
    strcpy_s(reg->name, 32, name_.c_str());
    sendMessage(msg, index, size);
    if (!waitForResponse(msg, 20000)) {
        callback_->logError([]() { return "Unable to connect to websocket_proxy. timeout"; });
        return false;
    }

    last_server_heartbeat_time_ = get_timestamp();
    if (msg->status.load(std::memory_order_relaxed) == Message::Status::FAILED) {
        callback_->logError([reg]() { return reg->err; });
        return false;
    }
    server_pid_.store(reg->server_pid, std::memory_order_release);
    callback_->logInfo([reg]() { return std::format("Proxy server connected, pid={}", reg->server_pid); });
    return true;
}

inline void WebsocketProxyClient::unregister() {
    auto [msg, index, size] = reserveMessage();
    msg->type = Message::Type::Unregister;
    sendMessage(msg, index, size);
    server_pid_.store(0, std::memory_order_release);
    callback_->logInfo([this]() { return std::format("Unregistered, pid={}", pid_); });
}

inline std::pair<uint64_t, bool> WebsocketProxyClient::openWebSocket(const std::string& url, const std::string &api_key) {
    auto msg = sendOpenMessage(url, api_key);
    if (!msg) {
        return std::make_pair(0, false);
    }

    if (!waitForResponse(msg, 30000)) {
        callback_->logError([]() { return "Open Websocket timedout"; });
        return std::make_pair(0, false);
    }

    auto req = reinterpret_cast<WsOpen*>(msg->data);
    last_server_heartbeat_time_ = get_timestamp();
    if (msg->status.load(std::memory_order_relaxed) == Message::Status::FAILED) {
        callback_->logError([req]() { return req->err; });
        return std::make_pair(0, false);
    }
    
    callback_->logDebug([req]() { return std::format("ws connected. id={}, new={}", req->id, req->new_connection); });
    return std::make_pair(req->id, req->new_connection);
}

inline bool WebsocketProxyClient::openWebSocketAsync(const std::string& url, const std::string &api_key) {
    return sendOpenMessage(url, api_key);
}

inline Message* WebsocketProxyClient::sendOpenMessage(const std::string& url, const std::string &api_key) {
    if (url.size() > 255) {
        callback_->logError([]() { return "URL is to long. limit is 255 character"; });
        return nullptr;
    }

    if (!server_pid_.load(std::memory_order_relaxed)) {
        if (!connect()) {
            return nullptr;
        }
    }

    auto [msg, index, size] = reserveMessage<WsOpen>();
    msg->type = Message::Type::OpenWs;
    auto req = reinterpret_cast<WsOpen*>(msg->data);
    strcpy_s(req->url, 512, url.c_str());
    strcpy_s(req->api_key, 512, api_key.c_str());
    
    sendMessage(msg, index, size);
    return msg;
}

inline bool WebsocketProxyClient::closeWebSocket(uint64_t id) {
    auto [msg, index, size] = reserveMessage<WsClose>();
    msg->type = Message::Type::CloseWs;
    auto req = reinterpret_cast<WsClose*>(msg->data);
    req->id = !id ? id_ : id;
    // log_(L_INFO, "Close ws " + std::to_string(req->id));
    sendMessage(msg, index, size);
    if (!waitForResponse(msg)) {
        callback_->logDebug([]() { return "Close ws timedout"; });
        return false;
    }
    unregister();
    return true;
}

inline bool WebsocketProxyClient::subscribe(uint64_t id, const std::string& symbol, const char* subscription_request, uint32_t request_len, SubscriptionType type, bool& existing) {
    auto [msg, index, size] = reserveMessage<WsSubscription>(request_len);
    msg->type = Message::Type::Subscribe;
    auto req = reinterpret_cast<WsSubscription*>(msg->data);
    req->request_len = request_len;
    req->id = id;
    req->type = type;
    memcpy(&req->symbol[0], symbol.c_str(), symbol.size());
    memcpy(req->request, subscription_request, request_len);
    sendMessage(msg, index, size);
    if (!waitForResponse(msg)) {
        callback_->logError([&symbol]() { return std::format("Subscribe {} timeout", symbol); });
        return false;
    }
    return true;
}

inline bool WebsocketProxyClient::unsubscribe(uint64_t id, const std::string& symbol, const char* unsubscription_request, uint32_t request_len) {
    auto [msg, index, size] = reserveMessage<WsSubscription>(request_len);
    msg->type = Message::Type::Unsubscribe;
    auto req = reinterpret_cast<WsSubscription*>(msg->data);
    req->request_len = request_len;
    req->id = id;
    req->existing = false;
    memcpy(&req->symbol[0], symbol.c_str(), symbol.size());
    memcpy(req->request, unsubscription_request, request_len);
    sendMessage(msg, index, size);
    return true;
}

inline void WebsocketProxyClient::send(uint64_t id, const char* data, uint32_t len) {
    auto [msg, index, size] = reserveMessage<WsRequest>(len);
    msg->type = Message::Type::WsRequest;
    auto req = reinterpret_cast<WsRequest*>(msg->data);
    req->len = len;
    req->id = id;
    memcpy(req->data, data, len);
    sendMessage(msg, index, size);
}

inline void WebsocketProxyClient::doWork() {
    run_ = std::make_shared<std::atomic_bool>(true);
    std::shared_ptr<std::atomic_bool> run = run_;
    while (run->load(std::memory_order_relaxed)) {
        auto server_pid = server_pid_.load(std::memory_order_relaxed);
        if (!server_pid) {
            // not connected yet
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
            continue;
        }
        auto now = get_timestamp();
        auto result = server_queue_->read(server_queue_index_);
        if (result.first) {
            auto msg = reinterpret_cast<Message*>(result.first);
            //log_(L_DEBUG, std::to_string(msg->type));
            last_server_heartbeat_time_ = now;
            if (server_pid != msg->pid) {
                continue;
            }
            switch (msg->type) {
            case Message::Type::OpenWs:
                handleWsOpen(msg);
                break;
            case Message::Type::CloseWs:
                handleWsClose(msg);
                break;
            case Message::Type::WsError:
                handleWsError(msg);
                break;
            case Message::Type::WsData:
                handleWsData(msg);
                break;
            }
        }

        bool heartbeat_sent = sendHeartbeat(now);

        if (!result.first) {
            if (last_server_heartbeat_time_ && (now - last_server_heartbeat_time_) > HEARTBEAT_TIMEOUT) {
                callback_->logInfo([this, now, server_pid]() { return std::format("Server {}  heatbeat timeout. now={}, last_seen={}", server_pid, now, last_server_heartbeat_time_); });
                server_pid_.store(0, std::memory_order_release);
                callback_->onWebsocketProxyServerDisconnected();
                if (!websockets_.empty()) {
                    for (auto& id : websockets_) {
                        callback_->onWebsocketClosed(id);
                    }
                    websockets_.clear();
                }
            }
            else if (!heartbeat_sent) {
                std::this_thread::yield();
            }
        }
    }
    callback_->logDebug([]() { return "WebsocketProxyClient work thread exit"; });
}

inline bool WebsocketProxyClient::sendHeartbeat(uint64_t now) {
    if (server_pid_.load(std::memory_order_relaxed) && (now - last_heartbeat_time_) > HEARTBEAT_INTERVAL) {
        auto [msg, index, size] = reserveMessage();
        msg->type = Message::Type::Heartbeat;
        sendMessage(msg, index, size);
        return true;
    }
    return false;
}

inline void WebsocketProxyClient::handleWsOpen(Message* msg) {
    auto open = reinterpret_cast<WsOpen*>(msg->data);
    callback_->logDebug([open]() { return std::format("handleWsOPen, initiator={}", open->client_pid); });
    if (open->client_pid == pid_) {
        websockets_.emplace(open->id);
        callback_->onWebsocketOpened(open->id);
    }
}

inline void WebsocketProxyClient::handleWsClose(Message* msg) {
    auto close = reinterpret_cast<WsClose*>(msg->data);
    auto it = websockets_.find(close->id);
    if (it != websockets_.end()) {
        websockets_.erase(close->id);
        callback_->onWebsocketClosed(close->id);
    }
    else {
        callback_->logDebug([close]() { return std::format("Ws closed. socket not found. id={}", close->id); });
    }
}

inline void WebsocketProxyClient::handleWsError(Message* msg) {
    auto err = reinterpret_cast<WsError*>(msg->data);
    auto it = websockets_.find(err->id);
    if (it != websockets_.end()) {
        callback_->onWebsocketError(err->id, err->err, err->len);
    }
    else {
        std::string error;
        if (err->len) {
            error.append(err->err, err->len);
        }
        callback_->logDebug([err, &error]() { return std::format("Ws error. socket not found. id={} err={}", err->id, error); });
    }
}

inline void WebsocketProxyClient::handleWsData(Message* msg) {
    auto data = reinterpret_cast<WsData*>(msg->data);
    auto it = websockets_.find(data->id);
    if (it != websockets_.end()) {
        callback_->onWebsocketData(data->id, data->data, data->len, data->remaining);
    }
    else {
        callback_->logDebug([data]() { return std::format("Ws data. socket not found. id={}", data->id); });
        // log_(L_DEBUG, );
    }
}

template<typename T>
inline std::tuple<Message*, uint64_t, uint32_t> WebsocketProxyClient::reserveMessage(uint32_t data_size) {
    auto size = get_message_size<T>(data_size);
    auto index = client_queue_->reserve(size);
    auto msg = reinterpret_cast<Message*>((*(client_queue_.get()))[index]);
    memset(msg, 0, size);
    msg->pid = pid_;
    return std::make_tuple(msg, index, size);
}

inline std::tuple<Message*, uint64_t, uint32_t> WebsocketProxyClient::reserveMessage() {
    uint32_t size = sizeof(Message);
    auto index = client_queue_->reserve(size);
    auto msg = reinterpret_cast<Message*>((*(client_queue_.get()))[index]);
    memset(msg, 0, size);
    msg->pid = pid_;
    return std::make_tuple(msg, index, size);
}

}