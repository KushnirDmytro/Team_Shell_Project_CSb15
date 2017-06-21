//
// Created by d1md1m on 14.06.17.
//


#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "Env/ConsoleView.h"


using boost::asio::ip::tcp;

string my_read_line(void)
{
    string buffer;
    getline(std::cin,buffer);
    return buffer;
}

int main(int argc, char* argv[])
{
    env::ConsoleView* example = new env::ConsoleView(nullptr, nullptr);
    try
    {
        while (true) {
            if (argc < 3) {
                std::cerr << "Usage: client <host>" << std::endl;
                return 1;
            }

            boost::asio::io_service io_service;

            tcp::resolver resolver(io_service);
            tcp::resolver::query queryTCP(argv[1], "2017");
            tcp::resolver::iterator endpoint_iterator = resolver.resolve(queryTCP);

            tcp::socket socketTCP(io_service);
            boost::asio::connect(socketTCP, endpoint_iterator);


            if ( true || (strcmp(argv[2], "h") == 0) || (strcmp(argv[2], "d") == 0) || (strcmp(argv[2], "t") == 0) ){
                std::string m_SendBuffer = argv[2];

                string str = my_read_line();

                printf("TCP CLIENT socket is open %d\n socket addr[%s]\n socket %d\n", socketTCP.is_open(),
                socketTCP.remote_endpoint().address().to_string().c_str(),
                       socketTCP.remote_endpoint().port());

                std::cout << "console: ";
                std::cout.write(str.data(), str.length());
                std::cout.write("\n", 1);

                std::flush(std::cout);

                //std::string m_SendBuffer = argv[2];


                socketTCP.send(boost::asio::buffer(str, str.size()));

                boost::array<char, 256> buf;
                boost::system::error_code error;

                size_t len = socketTCP.read_some(boost::asio::buffer(buf), error);

                std::cout << "Server: ";
                std::cout.write(buf.data(), len);
                std::cout.write("\n", 1);

                std::flush(std::cout);



//                char reply[msgSize ];
//                boost::asio::ip::udp::endpoint sender_endpoint;
//                size_t reply_length = socketTCP.receive(
//                        boost::asio::buffer(reply, m_SendBuffer.length()));
//
//                std::cout << "Server: ";
//                std::cout.write(reply, reply_length);
//                std::cout << "\n";
//
//                std::flush(std::cout);
            }

//            if (strcmp(argv[2], "m") == 0) {
//
//                if (argc != 4){
//                    std::cerr << "Usage: undefined argument for 'm'" << std::endl;
//                    return 1;
//                }
//
//                std::string m_SendBuffer = "m04";
//                socketTCP.send(boost::asio::buffer(m_SendBuffer, m_SendBuffer.size()));
//                size_t msgSize = 4;
//                boost::system::error_code error;
//
//
//                m_SendBuffer = argv[3];
//
//                socketTCP.send(boost::asio::buffer(m_SendBuffer, msgSize) );
//                //boost::asio::deadline_timer timer1_(io_service, boost::posix_time::seconds(1));
//                //timer1_.wait();
//
//                char reply[msgSize ];
//                boost::asio::ip::udp::endpoint sender_endpoint;
//                size_t reply_length = socketTCP.receive(
//                        boost::asio::buffer(reply, m_SendBuffer.length()));
//
//                std::cout << "Server: ";
//                std::cout.write(reply, reply_length);
//                std::cout << "\n";
//
//                std::flush(std::cout);
//
//            }


          //  socketTCP.close();
            boost::asio::deadline_timer timer1_(io_service, boost::posix_time::seconds(1));
            timer1_.wait();
        }

    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }


    return 0;
}