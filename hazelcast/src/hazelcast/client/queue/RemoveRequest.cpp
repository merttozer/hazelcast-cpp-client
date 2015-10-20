//
// Created by sancar koyunlu on 9/4/13.
// Copyright (c) 2013 hazelcast. All rights reserved.


#include "hazelcast/client/queue/RemoveRequest.h"
#include "hazelcast/client/queue/QueuePortableHook.h"
#include "hazelcast/client/serialization/PortableWriter.h"


namespace hazelcast {
    namespace client {
        namespace queue {
            RemoveRequest::RemoveRequest(const std::string& name, const serialization::pimpl::Data& data)
            : name(name)
            , data(data) {

            }

            int RemoveRequest::getFactoryId() const {
                return QueuePortableHook::F_ID;
            }

            int RemoveRequest::getClassId() const {
                return QueuePortableHook::REMOVE;
            }


            void RemoveRequest::write(serialization::PortableWriter& writer) const {
                writer.writeUTF("n", &name);
                writer.writeLong("t", 0);
                serialization::ObjectDataOutput& out = writer.getRawDataOutput();
                out.writeData(&data);
            }
        }
    }
}

