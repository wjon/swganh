// This file is part of SWGANH which is released under the MIT license.
// See file LICENSE or go to http://swganh.com/LICENSE

#ifndef ANH_NETWORK_SOE_SERVER_H_
#define ANH_NETWORK_SOE_SERVER_H_

#include <cstdint>
#include <memory>

#include <boost/asio.hpp>

#include "anh/network/soe/server_interface.h"

namespace anh {

class ByteBuffer;

namespace network {
namespace soe {

// FORWARD DECLARATION
class Session;

/**
 * @brief An SOE Protocol Service.
 *
 * 
 */
class Server : public ServerInterface {
public:
    explicit Server(boost::asio::io_service& io_service);

    ~Server();

    /**
     * @brief Starts the SOE Frontend Service.
     * 
     * @parama port The port to listen for messages on.
     */
    void Start(uint16_t port);
    
    /**
     * @brief
     */
    void Shutdown(void);
    
    /**
     * @brief Sends a message on the wire to the target endpoint.
     */
    void SendTo(const boost::asio::ip::udp::endpoint& endpoint, anh::ByteBuffer buffer);

    boost::asio::ip::udp::socket* socket();
    
    uint32_t max_receive_size();

    /**
     * Resolves a hostname to its ip.
     *
     * \param hostname The hostname to resolve.
     * \return The ip the hostname resolves to.
     */
    std::string Resolve(const std::string& hostname);
    
private:
    Server();
    
    void AsyncReceive();

    boost::asio::io_service& io_service_;
    boost::asio::strand strand_;
    
    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint current_remote_endpoint_;
    std::array<char, 496> recv_buffer_;
    
    uint64_t bytes_recv_;
    uint64_t bytes_sent_;
    uint32_t max_receive_size_;
};

}}} // namespace anh::network::soe

#endif // ANH_NETWORK_SOE_SERVER_H_
