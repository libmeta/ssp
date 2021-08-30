//
// Created by pzw on 21-1-23.
//

#pragma once

extern "C" {
#include <iperf3/iperf_config.h>
#include <iperf3/iperf_api.h>
}

namespace ssp ::base{

class IperfClient {
public:
    static std::string run(std::string host, int port = 8000, std::string tmp_path = "/storage/emulated/0/XLive/log/runtime/iperf3.XXXXXX") {
        if (host.empty() || port < 1024) {
            //xloge("invalid hostname  and port");
            return "";
        }
        auto test = iperf_new_test();
        if (test == nullptr) {
            //xloge("%s: failed to create test", host.c_str());
            return "";
        }
        iperf_defaults(test);
        iperf_set_verbose(test, 1);

        iperf_set_test_role(test, 'c');
        iperf_set_test_server_hostname(test, strdup(host.c_str()));
        iperf_set_test_server_port(test, port);
        /* iperf_set_test_reverse( test, 1 ); */
        iperf_set_test_omit(test, 3);
        iperf_set_test_duration(test, 5);
        iperf_set_test_reporter_interval(test, 1);
        iperf_set_test_stats_interval(test, 1);
        iperf_set_test_json_output(test, 1);
        iperf_set_test_template(test, strdup(tmp_path.c_str()));

        if (iperf_run_client(test) < 0) {
           // xloge("iperf_run_client %s: error - %s", host.c_str(), iperf_strerror(i_errno));
            iperf_free_test(test);
            return "";
        }

        std::string retJson;
        if (iperf_get_test_json_output_string(test)) {
            retJson = iperf_get_test_json_output_string(test);
//            fprintf(iperf_get_test_outfile(test), "%zd bytes of JSON", strlen(iperf_get_test_json_output_string(test)));
          //  xlogi("iperf_run_client :%s", retJson.c_str());
        }

        iperf_free_test(test);
        return retJson;
    }

};


#endif //XLIVE_IPERF_CLIENT_HPP
