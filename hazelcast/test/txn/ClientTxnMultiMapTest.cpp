//
// Created by sancar koyunlu on 9/18/13.
// Copyright (c) 2013 hazelcast. All rights reserved.


#include "ClientTxnMultiMapTest.h"
#include "HazelcastServerFactory.h"
#include "hazelcast/client/HazelcastClient.h"
#include "hazelcast/util/CountDownLatch.h"

namespace hazelcast {
    namespace client {

        class HazelcastClient;

        namespace test {
            using namespace iTest;

            ClientTxnMultiMapTest::ClientTxnMultiMapTest(HazelcastServerFactory &serverFactory)
            : iTestFixture<ClientTxnMultiMapTest>("ClientTxnMultiMapTest")
            , instance(serverFactory)
            , client(new HazelcastClient(clientConfig.addAddress(Address(serverFactory.getServerAddress(), 5701)))) {
            }


            ClientTxnMultiMapTest::~ClientTxnMultiMapTest() {
            }

            void ClientTxnMultiMapTest::addTests() {
                addTest(&ClientTxnMultiMapTest::testPutGetRemove, "testPutGetRemove");
            }

            void ClientTxnMultiMapTest::beforeClass() {
            }

            void ClientTxnMultiMapTest::afterClass() {
                client.reset();
                instance.shutdown();
            }

            void ClientTxnMultiMapTest::beforeTest() {
            }

            void ClientTxnMultiMapTest::afterTest() {
            }

            void putGetRemoveTestThread(hazelcast::util::ThreadArgs& args) {
                MultiMap<std::string, std::string > *mm = (MultiMap<std::string, std::string > *)args.arg0;
                HazelcastClient *client = (HazelcastClient *)args.arg1;
                hazelcast::util::CountDownLatch *latch = (hazelcast::util::CountDownLatch *)args.arg2;
                hazelcast::util::AtomicInt *error = (hazelcast::util::AtomicInt *)args.arg3;
                std::string key = hazelcast::util::IOUtil::to_string(hazelcast::util::Thread::getThreadID());
                client->getMultiMap<std::string, std::string>("testPutGetRemove").put(key, "value");
                TransactionContext context = client->newTransactionContext();
                try {
                    context.beginTransaction();
                    TransactionalMultiMap<std::string, std::string > multiMap = context.getMultiMap<std::string, std::string >("testPutGetRemove");
                    assertFalse(multiMap.put(key, "value"));
                    assertTrue(multiMap.put(key, "value1"));
                    assertTrue(multiMap.put(key, "value2"));
                    assertEqual(3, (int)multiMap.get(key).size());
                    context.commitTransaction();

                    assertEqual(3, (int)mm->get(key).size());

                    latch->countDown();
                } catch (std::exception &) {
                    ++(*error);
                    latch->countDown();
                } catch(iTest::iTestException&){
                    ++(*error);
                    latch->countDown();
                }
            }

            void ClientTxnMultiMapTest::testPutGetRemove() {

                MultiMap<std::string, std::string > mm = client->getMultiMap<std::string, std::string >("testPutGetRemove");
                int n = 10;
                hazelcast::util::CountDownLatch latch(n);
                hazelcast::util::AtomicInt error(0);
                std::vector<hazelcast::util::Thread*> threads(n);
                for (int i = 0; i < n; i++) {
                    threads[i] = new hazelcast::util::Thread(putGetRemoveTestThread, &mm, client.get(), &latch, &error);
                }
                assertTrue(latch.await(1));
                assertEqual(0, (int)error);
                for (int i = 0; i < n; i++) {
                    delete threads[i] ;
                }
            }


        }
    }
}

