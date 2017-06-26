//
// Created by d1md1m on 14.06.17.
//


#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "Env/ConsoleView.h"
#include "Shell_core/Utils/FileLaneIterator.h"


using boost::asio::ip::tcp;

string my_read_line(void) {
    string buffer;
    getline(std::cin, buffer);
    return buffer;
}

int main(int argc, char *argv[]) {
    env::ConsoleView *example = new env::ConsoleView(nullptr, nullptr);
    sh_core::utils::FileLaneIterator *fileLaneIterator;
    if (argc == 4 && strcmp(argv[2], "-f")==0) {
        fileLaneIterator = new sh_core::utils::FileLaneIterator(argv[3]);
    }
    try {

        string str;
        boost::asio::io_service io_service;

        tcp::resolver resolver(io_service);
        tcp::resolver::query queryTCP(argv[1], "5002");
//            tcp::resolver::query queryTCP("127.0.0.1", "2017");
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(queryTCP);

        tcp::socket socketTCP(io_service);
        boost::asio::connect(socketTCP, endpoint_iterator);

        printf("TCP CLIENT socket is open %d\n socket addr[%s]\n socket %d\n", socketTCP.is_open(),
               socketTCP.remote_endpoint().address().to_string().c_str(),
               socketTCP.remote_endpoint().port());

        while (true) {
            if (argc == 4 && strcmp(argv[2], "-f")==0) {
                fileLaneIterator->getNextString(&str);
                std::cout << "str = " << str << std::endl;
                if (!fileLaneIterator->fileIsReady()) {
                    str = my_read_line();
                    return 0;
                }
            } else{
                str = my_read_line();
            }


            std::cout << "console: ";
            std::cout.write(str.data(), str.length());
            std::cout.write("\n", 1);

            std::flush(std::cout);

            socketTCP.send(boost::asio::buffer(str, str.size()));

            str.clear();

            boost::array<char, 256> buf;
            boost::system::error_code error;

            size_t len = 0;

            do {
                len = socketTCP.read_some(boost::asio::buffer(buf), error);
            } while (!len);

            std::cout << "Server: ";
            std::cout.write(buf.data(), len);
            std::cout.write("\n", 1);

            std::flush(std::cout);


            //  socketTCP.close();
            //   boost::asio::deadline_timer timer1_(io_service, boost::posix_time::seconds(1));
            // timer1_.wait();
        }

    }
    catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }


    return 0;
}