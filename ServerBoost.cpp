
#include <cstdlib>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>

// forward declarations
class serverUDP;
class serverTCP;
class session;





const std::string currentTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[20];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%X", &tstruct);

    return buf;
}
const std::string currentDate() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[40];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);

    return buf;
}





using boost::asio::ip::tcp;

std::string my_read_line(void)
{
    std::string buffer;
    getline(std::cin,buffer);
    return buffer;
}

class session
{
public:
    session(boost::asio::io_service& io_service)
            : socket_(io_service)
    {
    }

    tcp::socket& socket()
    {
        return socket_;
    }

    void start()
    {
        boost::array<char, 256> buf;
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
                                boost::bind(&session::handle_read, this,
                                            boost::asio::placeholders::error,
                                            boost::asio::placeholders::bytes_transferred));
    }

private:
    void handle_read(const boost::system::error_code& error,
                     size_t bytes_transferred)
    {
        if (!error  )
        {
            if (strcmp(data_, "h") == 0){
                std::string st = "Hello\n";
                strcpy(data_, st.c_str());
                bytes_transferred = st.size();
            }
            if (strcmp(data_, "t") == 0){
                std::string st = currentTime();
                strcpy(data_, st.c_str());
                bytes_transferred = st.size();
            }

            if (strcmp(data_, "d") == 0){
                std::string st = currentDate();
                strcpy(data_, st.c_str());
                bytes_transferred = st.size();
            }


//            if (strcmp(data_, "m") == 0){
//
//                socket_.async_read_some(boost::asio::buffer(data_, 2),
//                                        boost::bind(&session::handle_read, this,
//                                                    boost::asio::placeholders::error,
//                                                    boost::asio::placeholders::bytes_transferred));
//
//                if (isdigit(data_[0]) && isdigit(data_[1]) ){
//                    printf("TCP got two digits %s\n", data_);
//
//                    bytes_transferred = atoi(data_);
//
//                    socket_.async_read_some(boost::asio::buffer(data_, atoi(data_)),
//                                            boost::bind(&session::handle_read, this,
//                                                        boost::asio::placeholders::error,
//                                                        boost::asio::placeholders::bytes_transferred));
//                    printf("TCP got following msg %s of len %u\n", data_, bytes_transferred );
//
//
//
//                }
//                else {
//                    printf("Two digits expected\n");
//                    delete this;
//                }
//            }

            if (strlen(data_) > 0)
            {
                printf("TCP sending %s to %s port %d\n",
                       data_,
                       socket_.remote_endpoint().address().to_string().c_str(),
                       socket_.remote_endpoint().port());


                boost::asio::async_write(socket_,
                                         boost::asio::buffer(data_, bytes_transferred),
                                         boost::bind(&session::handle_write, this,
                                                     boost::asio::placeholders::error));
                memset(data_, 0, sizeof data_);


                readSize = 0;
            }
        }
        else
        {
            delete this;
        }
    }

    void handle_write(const boost::system::error_code& error)
    {
        if (!error)
        {
            socket_.async_read_some(boost::asio::buffer(data_, readSize),
                                    boost::bind(&session::handle_read, this,
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
        }
        else
        {
            delete this;
        }
    }

    tcp::socket socket_;
    enum { max_length = 1024 };
    size_t readSize = 0;
    char data_[max_length];
};

class serverTCP
{
public:
    serverTCP(boost::asio::io_service& io_service, short port)
            : io_service_(io_service),
              acceptor_(io_service, tcp::endpoint(tcp::v4(), port))
    {
        start_accept();
    }

private:
    void start_accept()
    {
        session* new_session = new session(io_service_);
        acceptor_.async_accept(new_session->socket(),
                               boost::bind(&serverTCP::handle_accept, this, new_session,
                                           boost::asio::placeholders::error));
    }

    void handle_accept(session* new_session,
                       const boost::system::error_code& error)
    {
        if (!error)
        {
            new_session->start();
        }
        else
        {
            delete new_session;
        }

        start_accept();
    }

    boost::asio::io_service& io_service_;
    tcp::acceptor acceptor_;
};


int main(int argc, char* argv[])
{
    size_t sizePlaceholder = 0;
    try
    {

        boost::asio::io_service io_service;

        using namespace std; // For atoi.
        serverTCP sTCP(io_service, 2017);


        io_service.run();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}