#include <cstdlib>
#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <string>
#include <algorithm>
/* Include the C++ DDS API. */
#include "dds/dds.hpp"

/* Include data type and specific traits to be used with the C++ DDS API. */
#include "Data.hpp"
#include "init.hpp"

#include "myFunctions.hpp"

using namespace org::eclipse::cyclonedds;
using namespace std;

int Publisher::publish()
{
    try
    {
        auto writer = *writer_;
      
        // pick a random message to publish

        
        std::fstream myFile;
        myFile.open("../data/words.txt");

        int counter = 1;
        const int userID = 2;
        bool publish = true;
        while (publish && counter != 20)
        {
            // pick a random message to publish
            std::string message = random_message("../data/words.txt");

            /* Create a message to write. */
            HelloWorldData::Msg msg(userID, message, counter);

            /* Write the message. */
            std::thread::id this_id = std::this_thread::get_id();
            std::cout << "== [Application" << userID <<
            "/s"  << counter << 
            "] | " << message << " | " <<
            getpid() << "/" << this_id << std::endl;

            writer.write(msg);

            counter++;
            std::this_thread::sleep_for(std::chrono::milliseconds(2500));
        }
    }

    catch (const dds::core::Exception &e)
    {
        std::cerr << "=== [Publisher] Exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "=== [Publisher] Done." << std::endl;

    return EXIT_SUCCESS;
}

int main()
{
    Publisher application2;
    application2.init();
    application2.publish();
    return 0;
    
}
