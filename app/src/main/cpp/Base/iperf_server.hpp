//
// Created by pzw on 21-1-23.
//

#pragma once

#include <string>
#include <thread>
#include <atomic>

extern "C" {
#include <iperf3/iperf_config.h>
#include <iperf3/iperf_api.h>
}

namespace ssp ::base {

class IperfServer {
public:
    explicit IperfServer(int port = 8000, std::string tmp_path = "/storage/emulated/0/XLive/log/runtime/iperf3.XXXXXX")
        : mQuit(false), mPort(port), mTmpPath(tmp_path) {
        mServerThread = std::thread(&IperfServer::ServerThread, this);
    }

    ~IperfServer() {
        Quit();
        if (mServerThread.joinable()) {
            mServerThread.join();
        }
    }

    void Quit() {
        mQuit = true;
    }

    void ServerThread() {
        int consecutive_errors = 0;
        auto *test = iperf_new_test();
        if (test == nullptr) {
            //  xloge("failed to create test");
            return;
        }

        iperf_defaults(test);
        iperf_set_verbose(test, 1);
        iperf_set_test_role(test, 's');
        iperf_set_test_server_port(test, mPort);
        iperf_set_test_template(test, (char *) ("/storage/emulated/0/XLive/log/runtime/iperf3.XXXXXX"));
        while (!mQuit) {
            if (iperf_run_server(test) < 0) {
                //   xloge("iperf_run_server: %s", iperf_strerror(i_errno));
                ++consecutive_errors;
                if (consecutive_errors >= 5) {
                    //       xloge("iperf_run_server:too many errors, exiting");
                    consecutive_errors = 0;
//                    break;
                }
            } else {
                consecutive_errors = 0;
            }
            iperf_reset_test(test);
        }
        iperf_free_test(test);
    }

private:
    std::atomic<bool> mQuit;
    int mPort;
    std::string mTmpPath;
    std::thread mServerThread;


};

