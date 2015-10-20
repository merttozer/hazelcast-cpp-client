//
// Created by sancar koyunlu on 6/21/13.
// Copyright (c) 2013 hazelcast. All rights reserved.


#include "hazelcast/client/impl/PortableEntryEvent.h"
#include "hazelcast/client/serialization/PortableWriter.h"
#include "hazelcast/client/serialization/PortableReader.h"

namespace hazelcast {
    namespace client {
        namespace impl {

            const serialization::pimpl::Data& PortableEntryEvent::getKey() const {
                return key;
            }

            const serialization::pimpl::Data& PortableEntryEvent::getOldValue() const {
                return oldValue;
            }

            const serialization::pimpl::Data& PortableEntryEvent::getValue() const {
                return value;
            }

            const serialization::pimpl::Data& PortableEntryEvent::getMergingValue() const {
                return mergingValue;
            }

            std::string &PortableEntryEvent::getUuid() const {
                return *uuid;
            }

            EntryEventType PortableEntryEvent::getEventType() const {
                return eventType;
            }

            int PortableEntryEvent::getFactoryId() const {
                return protocol::SpiConstants::SPI_PORTABLE_FACTORY;
            }

            int PortableEntryEvent::getClassId() const {
                return protocol::SpiConstants::ENTRY_EVENT;
            }


            int PortableEntryEvent::getNumberOfAffectedEntries() const {
                return numberOfAffectedEntries;
            }

            void PortableEntryEvent::readPortable(serialization::PortableReader& reader) {
                eventType = reader.readInt("e");
                uuid = reader.readUTF("u");
                numberOfAffectedEntries = reader.readInt("n");
                serialization::ObjectDataInput &in = reader.getRawDataInput();
                key = in.readData();
                value = in.readData();
                oldValue = in.readData();
                mergingValue = in.readData();
            }
        }
    }
}

