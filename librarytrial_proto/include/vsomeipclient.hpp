#include <iomanip>
#include <iostream>
#include <sstream>

#include <condition_variable>
#include <thread>

#include <vsomeip/vsomeip.hpp>

typedef void (*res_cb)(const std::vector<uint8_t> &);

class vsomeipclient
{
public:
    vsomeipclient();
    void run();
    void register_callback12(res_cb cb);
    void on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available);
    void on_message(const std::shared_ptr<vsomeip::message> &_response);
    void start(uint16_t SAMPLE_SERVICE_ID, uint16_t SAMPLE_INSTANCE_ID, uint16_t SAMPLE_METHOD_ID);

private:
    res_cb responce_cb;
    std::shared_ptr<vsomeip::application> app;
    std::mutex mutex;
    std::condition_variable condition;
    uint16_t service_id_;
    uint16_t instance_id_;
    uint16_t method_id_;
    bool is_available = false;
};
