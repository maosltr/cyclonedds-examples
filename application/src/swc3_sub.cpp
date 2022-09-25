/*
 * Copyright(c) 2006 to 2021 ZettaScale Technology and others
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v. 2.0 which is available at
 * http://www.eclipse.org/legal/epl-2.0, or the Eclipse Distribution License
 * v. 1.0 which is available at
 * http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * SPDX-License-Identifier: EPL-2.0 OR BSD-3-Clause
 */
#include <cstdlib>
#include <iostream>
#include <chrono>
#include <thread>

/* Include the C++ DDS API. */
#include "dds/dds.hpp"

/* Include data type and specific traits to be used with the C++ DDS API. */
#include "Data.hpp"
#include "init.hpp"
//#include "listener.hpp"

using namespace org::eclipse::cyclonedds;
using namespace std;

int Subscriber_subscriber_listener::subscribe()
{
    try
    {
        // std::cout << "=== [Subscriber] Create reader." << std::endl;

        // /* First, a domain participant is needed.
        //  * Create one on the default domain. */
        // dds::domain::DomainParticipant participant(domain::default_id());

        // /* To subscribe to something, a topic is needed. */
        // dds::topic::Topic<HelloWorldData::Msg> topic(participant, "random_world");

        // /* create a listener */
        // SubListener sublistener;

        // /* a Mask is needed for the datareader listener */
        // // (source: ..\include\ddscxx\dds\core\status\State.hpp)
        // dds::core::status::StatusMask mask;
        // mask << dds::core::status::StatusMask::subscription_matched();
        // // mask << dds::core::status::StatusMask::data_available();
        // // mask << dds::core::status::StatusMask::all();

        // /* A reader also needs a subscriber. */
        // dds::sub::Subscriber subscriber(participant,
        //                                 participant.default_subscriber_qos(),
        //                                 &sublistener,
        //                                 mask);

        // /* Now, the reader can be created to subscribe to a HelloWorld message. */
        // dds::sub::DataReader<HelloWorldData::Msg> reader(subscriber, topic);




        auto reader = *reader_;

        int topics_counter = 0;
        while (topics_counter < 100)
        {

            dds::sub::LoanedSamples<HelloWorldData::Msg> samples;

            /* Try taking samples from the reader. */
            samples = reader.take();
            // sublistener.data_available = false;

            dds::sub::LoanedSamples<HelloWorldData::Msg>::const_iterator it;
            for (it = samples.begin(); it != samples.end(); ++it)
            {

                const HelloWorldData::Msg &msg = it->data();
                const dds::sub::SampleInfo &info = it->info();
                if (info.valid())
                {

                    // Use sample data and meta information.

                    std::thread::id this_id = std::this_thread::get_id();
                    std::cout << "== [swc3/swc" << msg.userID()  << "-s" << msg.counter() << "] | " << msg.message() << " | " << getpid() << "/" << this_id << std::endl;
                    
                    // simulating a network load resulting in missing a published topic
                    std::this_thread::sleep_for(std::chrono::milliseconds(2000));

                    topics_counter++;
                }
            }
        }
    }

    catch (const dds::core::Exception &e)
    {
        std::cerr << "=== [Subscriber] DDS exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (const std::exception &e)
    {
        std::cerr << "=== [Subscriber] C++ exception: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "=== [Subscriber] Done." << std::endl;

    return EXIT_SUCCESS;
};

int main()
{
    Subscriber_subscriber_listener swc3;
    swc3.init();
   
    swc3.subscribe();
    return 0;
}
