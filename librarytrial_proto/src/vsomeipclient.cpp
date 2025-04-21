#include"vsomeipclient.hpp"
void vsomeipclient::register_callback12(res_cb cb)
{
    responce_cb = cb;
}

vsomeipclient::vsomeipclient()
{
    app = vsomeip::runtime::get()->create_application("Hello");
    app->init();
}
void vsomeipclient::start(uint16_t SAMPLE_SERVICE_ID, uint16_t SAMPLE_INSTANCE_ID, uint16_t SAMPLE_METHOD_ID)
{
    service_id_ = SAMPLE_SERVICE_ID;
    instance_id_ = SAMPLE_INSTANCE_ID;
    method_id_ = SAMPLE_METHOD_ID;
    app->register_availability_handler(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID, std::bind(&vsomeipclient::on_availability, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    app->register_message_handler(service_id_, instance_id_, method_id_, std::bind(&vsomeipclient::on_message, this, std::placeholders::_1));

    app->request_service(SAMPLE_SERVICE_ID, SAMPLE_INSTANCE_ID);
    std::thread sender(&vsomeipclient::run, this);
    sender.detach();
    app->start();
}
void vsomeipclient::on_availability(vsomeip::service_t _service, vsomeip::instance_t _instance, bool _is_available)
{
    std::cout << "CLIENT: Service ["
              << std::setw(4) << std::setfill('0') << std::hex << _service << "." << _instance
              << "] is "
              << (_is_available ? "available." : "NOT available.")
              << std::endl;
    if (_is_available)
    {
        is_available = true;
        std::lock_guard<std::mutex> lock(mutex);
        condition.notify_one();
    }
}
void vsomeipclient::on_message(const std::shared_ptr<vsomeip::message> &_response)
{

    std::shared_ptr<vsomeip::payload> its_payload = _response->get_payload();
    vsomeip::length_t l = its_payload->get_length();

    std::vector<uint8_t> data(
        its_payload->get_data(),
        its_payload->get_data() + its_payload->get_length());
    if (responce_cb)
        responce_cb(data);
    // Get payload
    //   std::stringstream ss;
    //   for (vsomeip::length_t i=0; i<l; i++) {
    //      ss << std::setw(2) << std::setfill('0') << std::hex
    //         << (int)*(its_payload->get_data()+i) << " ";
    //   }

    //   std::cout << "CLIENT: Received message with Client/Session ["
    //       << std::setw(4) << std::setfill('0') << std::hex << _response->get_client() << "/"
    //       << std::setw(4) << std::setfill('0') << std::hex << _response->get_session() << "] "
    //       << ss.str() << std::endl;
}

void vsomeipclient::run()
{
    std::unique_lock<std::mutex> its_lock(mutex);
    condition.wait(its_lock, [this]
                   {
    // Add a flag if you want more control, for now always true is fine
    return is_available; });

    std::shared_ptr<vsomeip::message> request;
    request = vsomeip::runtime::get()->create_request();
    request->set_service(service_id_);
    request->set_instance(instance_id_);
    request->set_method(method_id_);

    std::shared_ptr<vsomeip::payload> its_payload = vsomeip::runtime::get()->create_payload();
    std::vector<vsomeip::byte_t> its_payload_data;
    for (vsomeip::byte_t i = 0; i < 10; i++)
    {
        its_payload_data.push_back(i % 256);
    }
    its_payload->set_data(its_payload_data);
    request->set_payload(its_payload);
    app->send(request);
}
