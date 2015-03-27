#include "ClientTopicTest.h"
#include "HazelcastServerFactory.h"
#include "hazelcast/client/HazelcastClient.h"

namespace hazelcast {
    namespace client {

        class HazelcastClient;

        namespace test {
            using namespace iTest;

            ClientTopicTest::ClientTopicTest(HazelcastServerFactory &serverFactory)
            : iTestFixture<ClientTopicTest>("ClientTopicTest")
            , instance(serverFactory)
            , client(new HazelcastClient(clientConfig.addAddress(Address(serverFactory.getServerAddress(), 5701))))
            , topic(new ITopic<std::string>(client->getTopic<std::string>("ClientTopicTest"))) {
            }


            void ClientTopicTest::addTests() {
                addTest(&ClientTopicTest::testTopicListeners, "testTopicListeners");
            }

            void ClientTopicTest::beforeClass() {
            }

            void ClientTopicTest::afterClass() {
                client.reset();
                instance.shutdown();
            }

            void ClientTopicTest::beforeTest() {
            }

            void ClientTopicTest::afterTest() {
            }

            class MyMessageListener {
            public:
                MyMessageListener(hazelcast::util::CountDownLatch &latch)
                :latch(latch) {

                }

                void onMessage(topic::Message<std::string> message) {
                    latch.countDown();
                }

            private:
                hazelcast::util::CountDownLatch &latch;
            };

            void ClientTopicTest::testTopicListeners() {

                hazelcast::util::CountDownLatch latch(10);
                MyMessageListener listener(latch);
                std::string id = topic->addMessageListener(listener);

                for (int i = 0; i < 10; i++) {
                    topic->publish(std::string("naber") + hazelcast::util::IOUtil::to_string(i));
                }
                assertTrue(latch.await(20 ));
                topic->removeMessageListener(id);

            }
        }
    }
}

