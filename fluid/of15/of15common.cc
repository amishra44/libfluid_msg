#include "of15common.hh"

namespace fluid_msg {

namespace of15 {

HelloElem::HelloElem(uint16_t type, uint16_t length) {
    this->type_ = type;
    this->length_ = length;
}

bool HelloElem::operator==(const HelloElem &other) const {
    return ((this->type_ == other.type_) && (this->length_ == other.length_));
}

bool HelloElem::operator!=(const HelloElem &other) const {
    return !(*this == other);
}

HelloElemVersionBitmap::HelloElemVersionBitmap(std::list<uint32_t> bitmaps)
    : HelloElem(of15::OFPHET_VERSIONBITMAP,
        sizeof(struct of15::ofp_hello_elem_versionbitmap)) {
    this->bitmaps_ = bitmaps;
    this->length_ += bitmaps.size() * sizeof(uint32_t);
}

bool HelloElemVersionBitmap::operator==(
    const HelloElemVersionBitmap &other) const {
    return ((HelloElem::operator==(other)) && (this->bitmaps_ == other.bitmaps_));
}

bool HelloElemVersionBitmap::operator!=(
    const HelloElemVersionBitmap &other) const {
    return !(*this == other);
}

void HelloElemVersionBitmap::add_bitmap(uint32_t bitmap) {
    this->bitmaps_.push_back(bitmap);
    this->length_ += sizeof(uint32_t);
}

size_t HelloElemVersionBitmap::pack(uint8_t* buffer) {
    struct of15::ofp_hello_elem_versionbitmap *elem =
        (struct of15::ofp_hello_elem_versionbitmap *) buffer;
    elem->type = hton16(this->type_);
    elem->length = hton16(this->length_);
    uint8_t *p = buffer + sizeof(struct of15::ofp_hello_elem_versionbitmap);
    for (std::list<uint32_t>::iterator it = this->bitmaps_.begin();
        it != this->bitmaps_.end(); it++) {
        uint32_t bitmap = hton32(*it);
        memcpy(p, &bitmap, sizeof(uint32_t));
        p += sizeof(uint32_t);
    }
    return 0;
}

of_error HelloElemVersionBitmap::unpack(uint8_t* buffer) {
    struct of15::ofp_hello_elem_versionbitmap *elem =
        (struct of15::ofp_hello_elem_versionbitmap *) buffer;
    this->type_ = ntoh16(elem->type);
    this->length_ = ntoh16(elem->length);
    uint32_t bitmaps;
    memcpy(&bitmaps, elem->bitmaps, sizeof(uint32_t));
    uint8_t *p = buffer + sizeof(struct of15::ofp_hello_elem_versionbitmap);
    size_t len = this->length_
        - sizeof(struct of15::ofp_hello_elem_versionbitmap);
    while (len) {
        uint32_t bitmap = ntoh32((*(uint32_t*) p));
        this->bitmaps_.push_back(bitmap);
        p += sizeof(uint32_t);
        len -= sizeof(uint32_t);
    }
    return 0;
}

PortDescProp::PortDescProp(uint16_t type) {
    this->type_ = type;
    this->length_ = sizeof(struct of15::ofp_port_desc_prop_header);
}

bool PortDescProp::equals(const PortDescProp &other) {
    return ((*this == other));
}

bool PortDescProp::operator==(const PortDescProp &other) const {
    return ((this->type_ == other.type_) && (this->length_ == other.length_));
}

bool PortDescProp::operator!=(const PortDescProp &other) const {
    return !(*this == other);
}

size_t PortDescProp::pack(uint8_t* buffer) {
    struct of15::ofp_port_desc_prop_header *fp =
        (struct of15::ofp_port_desc_prop_header*) buffer;
    fp->type = hton16(this->type_);
    fp->length = hton16(this->length_);
    return length();
}

of_error PortDescProp::unpack(uint8_t* buffer) {
    struct of15::ofp_port_desc_prop_header *fp =
        (struct of15::ofp_port_desc_prop_header*) buffer;
    this->type_ = ntoh16(fp->type);
    this->length_ = ntoh16(fp->length);
    return 0;
}

PortDescPropEthernet::PortDescPropEthernet(uint32_t type, uint32_t curr,
        uint32_t advertised, uint32_t supported, uint32_t peer,
        uint32_t curr_speed, uint32_t max_speed)
    : PortDescProp(type) {
    this->type_ = type;
    this->curr_ = curr;
    this->advertised_ = advertised;
    this->supported_ = supported;
    this->peer_ = peer;
    this->curr_speed_ = curr_speed;
    this->max_speed_ = max_speed;
    this->length_ += sizeof(of15::ofp_port_desc_prop_ethernet);
}

bool PortDescPropEthernet::equals(const PortDescProp &other) {
    if (const PortDescPropEthernet * prop =
        dynamic_cast<const PortDescPropEthernet *>(&other)) {
        return ((PortDescProp::equals(other))
            && (this->curr_ == prop->curr_)
            && (this->advertised_ == prop->advertised_)
            && (this->supported_ == prop->supported_)
            && (this->peer_ == prop->peer_)
            && (this->curr_speed_ == prop->curr_speed_)
            && (this->max_speed_ == prop->max_speed_));
    }
    else {
        return false;
    }
}

size_t PortDescPropEthernet::pack(uint8_t* buffer) {
    struct of15::ofp_port_desc_prop_ethernet *prop =
        (struct of15::ofp_port_desc_prop_ethernet*) buffer;
    PortDescProp::pack(buffer);
    prop->curr = hton32(this->curr_);
    prop->advertised = hton32(this->advertised_);
    prop->supported = hton32(this->supported_);
    prop->peer = hton32(this->peer_);
    prop->curr_speed = hton32(this->curr_speed_);
    prop->max_speed = hton32(this->max_speed_);

    return length();
}

of_error PortDescPropEthernet::unpack(uint8_t* buffer) {
    struct of15::ofp_port_desc_prop_ethernet *prop =
        (struct of15::ofp_port_desc_prop_ethernet*) buffer;
    PortDescProp::unpack(buffer);
    this->curr_ = ntoh32(prop->curr);
    this->advertised_ = ntoh32(prop->advertised);
    this->supported_ = ntoh32(prop->supported);
    this->peer_ = ntoh32(prop->peer);
    this->curr_speed_ = ntoh32(prop->curr_speed);
    this->max_speed_ = ntoh32(prop->max_speed);
    return 0;
}

PortDescPropOXM::PortDescPropOXM(uint16_t type,
    std::vector<uint32_t> oxm_ids)
    : PortDescProp(type) {
    this->oxm_ids_ = oxm_ids;
    this->length_ += oxm_ids_.size() * sizeof(uint32_t);
}

bool PortDescPropOXM::equals(const PortDescProp &other) {
    if (const PortDescPropOXM * prop =
        dynamic_cast<const PortDescPropOXM *>(&other)) {
        return ((PortDescProp::equals(other))
            && (this->oxm_ids_ == prop->oxm_ids_));
    }
    else {
        return false;
    }
}

size_t PortDescPropOXM::pack(uint8_t* buffer) {
	PortDescProp::pack(buffer);
    uint8_t *p = buffer + sizeof(struct of15::ofp_port_desc_prop_header);
    for (std::vector<uint32_t>::iterator it = this->oxm_ids_.begin();
        it != this->oxm_ids_.end(); ++it) {
        memcpy(p, &(*it), sizeof(uint32_t));
        p += sizeof(uint32_t);
    }
    return length();
}

of_error PortDescPropOXM::unpack(uint8_t* buffer) {
    PortDescProp::unpack(buffer);
    uint8_t *p = buffer + sizeof(struct of15::ofp_port_desc_prop_header);
    size_t len = this->length_
        - sizeof(struct of15::ofp_port_desc_prop_header);
    while (len) {
        uint32_t *oxm_id = (uint32_t*) p;
        this->oxm_ids_.push_back(*oxm_id);
        p += sizeof(uint32_t);
        len -= sizeof(uint32_t);
    }
    return 0;
}

void PortDescPropOXM::oxm_ids(std::vector<uint32_t> oxm_ids) {
    this->oxm_ids_ = oxm_ids;
    this->length_ += oxm_ids_.size() * sizeof(uint32_t);
}

PortDescPropsList::PortDescPropsList(
    std::list<PortDescProp*> property_list) {
    this->property_list_ = property_list_;
    for (std::list<PortDescProp*>::const_iterator it =
        property_list.begin(); it != property_list.end(); ++it) {
        this->length_ += (*it)->length();
    }
}

PortDescPropsList::PortDescPropsList(const PortDescPropsList &other) {
    this->length_ = other.length_;
    for (std::list<PortDescProp*>::const_iterator it =
        other.property_list_.begin(); it != other.property_list_.end(); ++it) {
        this->property_list_.push_back((*it)->clone());
    }
}

PortDescPropsList::~PortDescPropsList() {
    this->property_list_.remove_if(PortDescProp::delete_all);
}

bool PortDescPropsList::operator==(const PortDescPropsList &other) const {
    std::list<PortDescProp*>::const_iterator ot =
        other.property_list_.begin();
    for (std::list<PortDescProp*>::const_iterator it =
        this->property_list_.begin(); it != this->property_list_.end();
        ++it, ++ot) {
        if (!((*it)->equals(**ot))) {
            return false;
        }
    }
    return true;
}

bool PortDescPropsList::operator!=(const PortDescPropsList &other) const {
    return !(*this == other);
}

size_t PortDescPropsList::pack(uint8_t* buffer) {
    uint8_t *p = buffer;
    for (std::list<PortDescProp*>::iterator it =
        this->property_list_.begin(), end = this->property_list_.end();
        it != end; it++) {
        (*it)->pack(p);
        p += (*it)->length();
    }
    return length();
}

of_error PortDescPropsList::unpack(uint8_t* buffer) {
    uint8_t *p = buffer;
    size_t len = this->length_;
    PortDescProp *prop;
    while (len) {
        uint16_t type = ntoh16(*((uint16_t*) p));
        prop = Port::make_port_desc_prop(type);
        prop->unpack(p);
        this->property_list_.push_back(prop);
        len -= prop->length();
        p += prop->length();
    }
    return 0;
}

PortDescPropsList& PortDescPropsList::operator=(PortDescPropsList other) {
    swap(*this, other);
    return *this;
}

void swap(PortDescPropsList& first, PortDescPropsList& second) {
    std::swap(first.length_, second.length_);
    std::swap(first.property_list_, second.property_list_);
}

void PortDescPropsList::property_list(
    std::list<PortDescProp*> property_list) {
    this->property_list_ = property_list;
    for (std::list<PortDescProp*>::const_iterator it =
        property_list.begin(); it != property_list.end(); ++it) {
        this->length_ += (*it)->length();
    }
}

void PortDescPropsList::add_property(PortDescProp* prop) {
    this->property_list_.push_back(prop);
    this->length_ += prop->length();
}

Port::Port(uint32_t port_no, EthAddress hw_addr, std::string name,
    uint32_t config, uint32_t state)
    : port_no_(port_no),
      hw_addr_(hw_addr),
      name_(name),
      config_(config),
      state_(state) {
    this->length_ = sizeof(struct of15::ofp_port);
}

bool Port::operator==(const Port &other) const {
    return ((this->port_no_ == other.port_no_)
        && (this->hw_addr_ == other.hw_addr_)
        && (this->name_ == other.name_)
        && (this->config_ == other.config_)
        && (this->state_ == other.state_));
}

bool Port::operator!=(const Port &other) const {
    return !(*this == other);
}

size_t Port::pack(uint8_t* buffer) {
    struct of15::ofp_port *port = (struct of15::ofp_port*) buffer;
    port->port_no = hton32(this->port_no_);
    port->length = hton16(this->length_);
    memcpy(port->hw_addr, this->hw_addr_.get_data(), OFP_ETH_ALEN);
    memset(port->name, 0x0, OFP_MAX_PORT_NAME_LEN);
    memcpy(port->name, this->name_.c_str(),
        this->name_.size() < OFP_MAX_PORT_NAME_LEN ?
            this->name_.size() : OFP_MAX_PORT_NAME_LEN);
    port->config = hton32(this->config_);
    port->state = hton32(this->state_);
    uint8_t *p = buffer + sizeof(struct of15::ofp_port);
    this->properties_.pack(p);
    return length();

}

of_error Port::unpack(uint8_t* buffer) {
    struct of15::ofp_port *port = (struct of15::ofp_port*) buffer;
    this->port_no_ = ntoh32(port->port_no);
    this->hw_addr_ = EthAddress(port->hw_addr);
    this->name_ = std::string(port->name);
    this->config_ = ntoh32(port->config);
    this->state_ = ntoh32(port->state);
    uint8_t *p = buffer + sizeof(struct of15::ofp_port);
    this->properties_.length(
        this->length_ - sizeof(struct of15::ofp_port));
    this->properties_.unpack(p);
    return 0;
}

void Port::properties(PortDescPropsList properties) {
    this->properties_ = properties;
    this->length_ += properties.length();
}

void Port::add_port_prop(PortDescProp* prop) {
    this->properties_.add_property(prop);
    this->length_ += prop->length();
}

PortDescProp* Port::make_port_desc_prop(uint16_t type) {
    if (type == OFPPDPT_ETHERNET) {
        return (PortDescProp *) new PortDescPropEthernet(type);
    }

    if (type == OFPPDPT_PIPELINE_INPUT || type == OFPPDPT_PIPELINE_OUTPUT) {
        return (PortDescProp *) new PortDescPropOXM(type);
    }
    return NULL;
}

//Aman
//QueuePropMinRate::QueuePropMinRate(uint16_t rate)
//    : QueuePropRate(of15::OFPQT_MIN_RATE, rate) {
//    this->len_ = sizeof(struct of15::ofp_queue_prop_min_rate);
//}
//
//bool QueuePropMinRate::equals(const QueueProperty &other) {
//    if (const QueuePropMinRate * prop =
//        dynamic_cast<const QueuePropMinRate *>(&other)) {
//        return ((QueuePropRate::equals(other)));
//    }
//    else {
//        return false;
//    }
//}
//
//size_t QueuePropMinRate::pack(uint8_t* buffer) {
//    struct of15::ofp_queue_prop_min_rate *qp =
//        (struct of15::ofp_queue_prop_min_rate *) buffer;
//    QueueProperty::pack(buffer);
//    qp->rate = hton16(this->rate_);
//    memset(qp->pad, 0x0, 6);
//    return this->len_;
//}
//
//of_error QueuePropMinRate::unpack(uint8_t* buffer) {
//    struct of15::ofp_queue_prop_min_rate *qp =
//        (struct of15::ofp_queue_prop_min_rate *) buffer;
//    QueueProperty::unpack(buffer);
//    this->rate_ = ntoh16(qp->rate);
//    return 0;
//}
//
//QueuePropMaxRate::QueuePropMaxRate(uint16_t rate)
//    : QueuePropRate(of15::OFPQT_MAX_RATE, rate) {
//    this->len_ = sizeof(struct of15::ofp_queue_prop_min_rate);
//}
//
//bool QueuePropMaxRate::equals(const QueueProperty &other) {
//    if (const QueuePropMaxRate * prop =
//        dynamic_cast<const QueuePropMaxRate *>(&other)) {
//        return ((QueuePropRate::equals(other)));
//    }
//    else {
//        return false;
//    }
//}
//
//size_t QueuePropMaxRate::pack(uint8_t* buffer) {
//    struct of15::ofp_queue_prop_min_rate *qp =
//        (struct of15::ofp_queue_prop_min_rate *) buffer;
//    QueueProperty::pack(buffer);
//    qp->rate = hton16(this->rate_);
//    memset(qp->pad, 0x0, 6);
//    return this->len_;
//}
//
//of_error QueuePropMaxRate::unpack(uint8_t* buffer) {
//    struct of15::ofp_queue_prop_min_rate *qp =
//        (struct of15::ofp_queue_prop_min_rate *) buffer;
//    QueueProperty::unpack(buffer);
//    this->rate_ = ntoh16(qp->rate);
//    return 0;
//}

// Handle other queue requests type
QueueExperimenter::QueueExperimenter(uint32_t experimenter)
    : QueueProperty(of15::OFPQDPT_EXPERIMENTER) { //Aman
    this->experimenter_ = experimenter;
    this->len_ = sizeof(struct of15::ofp_queue_desc_prop_min_rate); //Aman
}

size_t QueueExperimenter::pack(uint8_t* buffer) {
    struct of15::ofp_queue_desc_prop_experimenter *qp = //Aman
        (struct of15::ofp_queue_desc_prop_experimenter *) buffer; //Aman
    QueueProperty::pack(buffer);
    qp->experimenter = hton32(this->experimenter_);
//    memset(qp->pad, 0x0, 4); //Aman
    return this->len_;
}

of_error QueueExperimenter::unpack(uint8_t* buffer) {
    struct of15::ofp_queue_desc_prop_experimenter *qp = //Aman
        (struct of15::ofp_queue_desc_prop_experimenter *) buffer; //Aman
    QueueProperty::unpack(buffer);
    this->experimenter_ = ntoh32(qp->experimenter);
    return 0;
}

//Aman
//PacketQueue::PacketQueue()
//    : PacketQueueCommon(),
//      port_(0) {
//    this->len_ = sizeof(struct of15::ofp_packet_queue);
//}
//
//PacketQueue::PacketQueue(uint32_t queue_id, uint32_t port)
//    : PacketQueueCommon(queue_id) {
//    this->port_ = port;
//    this->len_ = sizeof(struct of15::ofp_packet_queue);
//}
//
//PacketQueue::PacketQueue(uint32_t queue_id, uint32_t port,
//    QueuePropertyList properties)
//    : PacketQueueCommon(queue_id) {
//    this->port_ = port;
//    this->properties_ = properties;
//    this->len_ = sizeof(struct of15::ofp_packet_queue) + properties.length();
//}
//
//bool PacketQueue::operator==(const PacketQueue &other) const {
//    return ((PacketQueueCommon::operator==(other))
//        && (this->port_ == other.port_));
//}
//
//bool PacketQueue::operator!=(const PacketQueue &other) const {
//    return !(*this == other);
//}
//
//size_t PacketQueue::pack(uint8_t* buffer) {
//    struct of15::ofp_packet_queue *pq = (struct of15::ofp_packet_queue*) buffer;
//    pq->queue_id = hton32(this->queue_id_);
//    pq->port = hton32(this->port_);
//    pq->len = hton16(this->len_);
//    memset(pq->pad, 0x0, 6);
//    uint8_t *p = buffer + sizeof(struct of15::ofp_packet_queue);
//    this->properties_.pack(p);
//    return this->len_;
//}
//
//of_error PacketQueue::unpack(uint8_t* buffer) {
//    struct of15::ofp_packet_queue *pq = (struct of15::ofp_packet_queue*) buffer;
//    this->queue_id_ = ntoh32(pq->queue_id);
//    this->port_ = ntoh32(pq->port);
//    this->len_ = ntoh16(pq->len);
//    uint8_t *p = buffer + sizeof(struct of15::ofp_packet_queue);
//    this->properties_.length(
//        this->len_ - sizeof(struct of15::ofp_packet_queue));
//    this->properties_.unpack13(p);
//    return 0;
//}

Bucket::Bucket()
    : length_(sizeof(struct of15::ofp_bucket)),
      weight_(0),
      watch_port_(0),
      watch_group_(0) {
}

Bucket::Bucket(uint16_t weight, uint32_t watch_port, uint32_t watch_group) {
    this->weight_ = weight;
    this->watch_port_ = watch_port;
    this->watch_group_ = watch_group;
    this->length_ = sizeof(struct of15::ofp_bucket);
}

Bucket::Bucket(uint16_t weight, uint32_t watch_port, uint32_t watch_group,
    ActionSet actions) {
    this->weight_ = weight;
    this->watch_port_ = watch_port;
    this->watch_group_ = watch_group;
    this->actions_ = actions;
    this->length_ = sizeof(struct of15::ofp_bucket) + actions.length();
}

bool Bucket::operator==(const Bucket &other) const {
    return ((this->length_ == other.length_) && (this->weight_ == other.weight_)
        && (this->watch_port_ == other.watch_port_)
        && (this->watch_group_ == other.watch_group_)
        && (this->actions_ == other.actions_));
}

bool Bucket::operator!=(const Bucket &other) const {
    return !(*this == other);
}

void Bucket::actions(ActionSet actions) {
    this->actions_ = actions;
    this->length_ += actions.length();
}

void Bucket::add_action(Action &action) {
    this->actions_.add_action(action);
    this->length_ += action.length();
}

void Bucket::add_action(Action* action) {
    this->actions_.add_action(action);
    this->length_ += action->length();
}

size_t Bucket::pack(uint8_t* buffer) {
    struct of15::ofp_bucket *b = (struct of15::ofp_bucket*) buffer;
    b->len = hton16(this->length_);
//Aman
//    b->weight = hton16(this->weight_);
//    b->watch_port = hton32(this->watch_port_);
//    b->watch_group = hton32(this->watch_group_);
//    memset(b->pad, 0x0, 4);
    this->actions_.pack(buffer + sizeof(struct of15::ofp_bucket));
    return 0;
}

of_error Bucket::unpack(uint8_t* buffer) {
    struct of15::ofp_bucket *b = (struct of15::ofp_bucket*) buffer;
    this->length_ = ntoh16(b->len);
//Aman
//    this->weight_ = ntoh16(b->weight);
//    this->watch_port_ = ntoh32(b->watch_port);
//    this->watch_group_ = ntoh32(b->watch_group);
    this->actions_.length(this->length_ - sizeof(struct of15::ofp_bucket));
    this->actions_.unpack(buffer + sizeof(struct of15::ofp_bucket));
    return 0;
}

FlowStats::FlowStats()
    : FlowStatsCommon(),
      flags_(0) {
    this->length_ = sizeof(struct of15::ofp_flow_stats)
        - sizeof(struct of15::ofp_match);
}

FlowStats::FlowStats(uint8_t table_id, uint32_t duration_sec,
    uint32_t duration_nsec, uint16_t priority, uint16_t idle_timeout,
    uint16_t hard_timeout, uint16_t flags, uint64_t cookie,
    uint64_t packet_count, uint64_t byte_count)
    : FlowStatsCommon(table_id, duration_sec, duration_nsec, priority,
        idle_timeout, hard_timeout, cookie, packet_count, byte_count) {
    this->flags_ = flags;
    this->length_ = sizeof(struct of15::ofp_flow_stats)
        - sizeof(struct of15::ofp_match);
}

bool FlowStats::operator==(const FlowStats &other) const {
    return ((FlowStatsCommon::operator==(other))
        && (this->flags_ == other.flags_)
        && (this->instructions_ == other.instructions_)
        && (this->match_ == other.match_));
}

bool FlowStats::operator!=(const FlowStats &other) const {
    return !(*this == other);
}

size_t FlowStats::pack(uint8_t* buffer) {
    size_t padding = ROUND_UP(
        sizeof(struct of15::ofp_flow_stats) - sizeof(struct of15::ofp_match)
            + this->match_.length(), 8)
        - (sizeof(struct of15::ofp_flow_stats) - sizeof(struct of15::ofp_match)
            + this->match_.length());
    struct of15::ofp_flow_stats *fs = (struct of15::ofp_flow_stats*) buffer;
    fs->length = hton16(this->length_);
    fs->table_id = this->table_id_;
//Aman
//    memset(&fs->pad, 0x0, 1);
//    fs->duration_sec = hton32(this->duration_sec_);
//    fs->duration_nsec = hton32(this->duration_nsec_);
    fs->priority = hton16(this->priority_);
//    fs->idle_timeout = hton16(this->idle_timeout_);
//    fs->hard_timeout = hton16(this->hard_timeout_);
//    fs->flags = hton16(this->flags_);
    memset(fs->pad2, 0x0, 4);
//    fs->cookie = hton64(this->cookie_);
//    fs->packet_count = hton64(this->packet_count_);
//    fs->byte_count = hton64(this->byte_count_);
    uint8_t *p =
        buffer
            + (sizeof(struct of15::ofp_flow_stats)
                - sizeof(struct of15::ofp_match));
    this->match_.pack(p);
    p += this->match_.length();
    memset(p, 0x0, padding);
    p += padding;
    this->instructions_.pack(p);
    return this->length_;
}

of_error FlowStats::unpack(uint8_t* buffer) {
    struct of15::ofp_flow_stats *fs = (struct of15::ofp_flow_stats*) buffer;
    this->length_ = ntoh16(fs->length);
    this->table_id_ = fs->table_id;
//Aman
//    this->duration_sec_ = ntoh32(fs->duration_sec);
//    this->duration_nsec_ = ntoh32(fs->duration_nsec);
    this->priority_ = ntoh16(fs->priority);
//    this->idle_timeout_ = ntoh16(fs->idle_timeout);
//    this->hard_timeout_ = ntoh16(fs->hard_timeout);
//    this->flags_ = ntoh16(fs->flags);
//    this->cookie_ = ntoh64(fs->cookie);
//    this->packet_count_ = ntoh64(fs->packet_count);
//    this->byte_count_ = ntoh64(fs->byte_count);
    uint8_t *p =
        buffer
            + (sizeof(struct of15::ofp_flow_stats)
                - sizeof(struct of15::ofp_match));
    this->match_.unpack(p);
    this->instructions_.length(
        this->length_
            - ((sizeof(struct of15::ofp_flow_stats)
                - sizeof(struct of15::ofp_match))
                + ROUND_UP(this->match_.length(), 8)));
    p += ROUND_UP(this->match_.length(), 8);
    this->instructions_.unpack(p);
    return 0;
}

void FlowStats::match(of15::Match match) {
    this->match_ = match;
    this->length_ += match.length();
    //Padding bytes 
    this->length_ = ROUND_UP(this->length_, 8);
}

OXMTLV * FlowStats::get_oxm_field(uint8_t field) {
    return this->match_.oxm_field(field);
}

void FlowStats::instructions(InstructionSet instructions) {
    this->instructions_ = instructions;
    this->length_ += instructions.length();
}

void FlowStats::add_instruction(Instruction* inst) {
    this->instructions_.add_instruction(inst);
    this->length_ += inst->length();
}

TableStats::TableStats()
    : TableStatsCommon() {
}

TableStats::TableStats(uint8_t table_id, uint32_t active_count,
    uint64_t lookup_count, uint64_t matched_count)
    : TableStatsCommon(table_id, active_count, lookup_count, matched_count) {
}

size_t TableStats::pack(uint8_t* buffer) {
    struct of15::ofp_table_stats *ts = (struct of15::ofp_table_stats*) buffer;
    ts->table_id = this->table_id_;
    memset(ts->pad, 0x0, 3);
    ts->active_count = hton32(this->active_count_);
    ts->lookup_count = hton64(this->lookup_count_);
    ts->matched_count = hton64(this->matched_count_);
    return 0;
}

of_error TableStats::unpack(uint8_t* buffer) {
    struct of15::ofp_table_stats *ts = (struct of15::ofp_table_stats*) buffer;
    this->table_id_ = ts->table_id;
    this->active_count_ = ntoh32(ts->active_count);
    this->lookup_count_ = ntoh64(ts->lookup_count);
    this->matched_count_ = ntoh64(ts->matched_count);
    return 0;
}

PortStats::PortStats()
    : PortStatsCommon(),
      port_no_(0),
      duration_sec_(0),
      duration_nsec_(0) {
}

PortStats::PortStats(uint32_t port_no, struct port_rx_tx_stats rx_tx_stats,
    struct port_err_stats err_stats, uint64_t collisions, uint32_t duration_sec,
    uint32_t duration_nsec)
    : PortStatsCommon(rx_tx_stats, err_stats, collisions) {
    this->port_no_ = port_no;
    this->duration_sec_ = duration_sec;
    this->duration_nsec_ = duration_nsec;
}

bool PortStats::operator==(const PortStats &other) const {
    return ((PortStatsCommon::operator==(other))
        && (this->port_no_ == other.port_no_)
        && (this->duration_sec_ == other.duration_sec_)
        && (this->duration_nsec_ == other.duration_nsec_));
}

bool PortStats::operator!=(const PortStats &other) const {
    return !(*this == other);
}

size_t PortStats::pack(uint8_t* buffer) {
    struct of15::ofp_port_stats *ps = (struct of15::ofp_port_stats*) buffer;
    ps->port_no = hton32(this->port_no_);
    memset(ps->pad, 0x0, 6);
    PortStatsCommon::pack(buffer + 8);
//    ps->collisions = hton64(this->collisions_); //Aman
    ps->duration_sec = hton32(this->duration_sec_);
    ps->duration_nsec = hton32(this->duration_nsec_);
    return 0;
}

of_error PortStats::unpack(uint8_t* buffer) {
    struct of15::ofp_port_stats *ps = (struct of15::ofp_port_stats*) buffer;
    this->port_no_ = ntoh32(ps->port_no);
    PortStatsCommon::unpack(buffer + 8);
//    this->collisions_ = ntoh64(ps->collisions); //Aman
    this->duration_sec_ = hton32(ps->duration_sec);
    this->duration_nsec_ = hton32(ps->duration_nsec);
    return 0;
}

QueueStats::QueueStats()
    : QueueStatsCommon(),
      port_no_(0),
      duration_sec_(0),
      duration_nsec_(0) {

}

QueueStats::QueueStats(uint32_t port_no, uint32_t queue_id, uint64_t tx_bytes,
    uint64_t tx_packets, uint64_t tx_errors, uint32_t duration_sec,
    uint32_t duration_nsec)
    : QueueStatsCommon(queue_id, tx_bytes, tx_packets, tx_errors) {
    this->port_no_ = port_no;
    this->duration_sec_ = duration_sec;
    this->duration_nsec_ = duration_nsec;
}

bool QueueStats::operator==(const QueueStats &other) const {
    return ((QueueStatsCommon::operator==(other))
        && (this->port_no_ == other.port_no_)
        && (this->duration_sec_ == other.duration_sec_)
        && (this->duration_nsec_ == other.duration_nsec_));
}
bool QueueStats::operator!=(const QueueStats &other) const {
    return !(*this == other);
}

size_t QueueStats::pack(uint8_t* buffer) {
    struct of15::ofp_queue_stats *qs = (struct of15::ofp_queue_stats*) buffer;
    qs->port_no = hton32(this->port_no_);
    qs->queue_id = hton32(this->queue_id_);
    qs->tx_bytes = hton64(this->tx_bytes_);
    qs->tx_packets = hton64(this->tx_packets_);
    qs->tx_errors = hton64(this->tx_errors_);
    qs->duration_sec = hton32(this->duration_sec_);
    qs->duration_nsec = hton32(this->duration_nsec_);
    return 0;
}

of_error QueueStats::unpack(uint8_t* buffer) {
    struct of15::ofp_queue_stats *qs = (struct of15::ofp_queue_stats*) buffer;
    this->port_no_ = ntoh32(qs->port_no);
    this->queue_id_ = ntoh32(qs->queue_id);
    this->tx_bytes_ = ntoh64(qs->tx_bytes);
    this->tx_packets_ = ntoh64(qs->tx_packets);
    this->tx_errors_ = ntoh64(qs->tx_errors);
    this->duration_sec_ = ntoh32(qs->duration_sec);
    this->duration_nsec_ = ntoh32(qs->duration_nsec);
    return 0;
}

BucketStats::BucketStats()
    : packet_count_(0),
      byte_count_(0) {

}

BucketStats::BucketStats(uint64_t packet_count, uint64_t byte_count) {
    this->packet_count_ = packet_count;
    this->byte_count_ = byte_count;
}

bool BucketStats::operator==(const BucketStats &other) const {
    return ((this->packet_count_ == other.packet_count_)
        && (this->byte_count_ == other.byte_count_));
}

bool BucketStats::operator!=(const BucketStats &other) const {
    return !(*this == other);
}

size_t BucketStats::pack(uint8_t* buffer) {
    struct of15::ofp_bucket_counter *bc =
        (struct of15::ofp_bucket_counter *) buffer;
    bc->packet_count = hton64(this->packet_count_);
    bc->byte_count = hton64(this->byte_count_);
    return 0;
}

of_error BucketStats::unpack(uint8_t* buffer) {
    struct of15::ofp_bucket_counter *bc =
        (struct of15::ofp_bucket_counter *) buffer;
    this->packet_count_ = ntoh64(bc->packet_count);
    this->byte_count_ = ntoh64(bc->byte_count);
    return 0;
}

GroupStats::GroupStats(uint32_t group_id, uint32_t ref_count,
    uint64_t packet_count, uint64_t byte_count, uint32_t duration_sec,
    uint32_t duration_nsec) {
    this->group_id_ = group_id;
    this->ref_count_ = ref_count;
    this->packet_count_ = packet_count;
    this->byte_count_ = byte_count;
    this->duration_sec_ = duration_sec;
    this->duration_nsec_ = duration_nsec;
    this->length_ = sizeof(struct of15::ofp_group_stats);
}

GroupStats::GroupStats(uint32_t group_id, uint32_t ref_count,
    uint64_t packet_count, uint64_t byte_count, uint32_t duration_sec,
    uint32_t duration_nsec, std::vector<BucketStats> bucket_stats) {
    this->group_id_ = group_id;
    this->ref_count_ = ref_count;
    this->packet_count_ = packet_count;
    this->byte_count_ = byte_count;
    this->duration_sec_ = duration_sec;
    this->duration_nsec_ = duration_nsec;
    this->bucket_stats_ = bucket_stats;
    this->length_ = sizeof(struct of15::ofp_group_stats)
        + bucket_stats.size() * sizeof(struct of15::ofp_bucket_counter);
}

bool GroupStats::operator==(const GroupStats &other) const {
    return ((this->group_id_ == other.group_id_)
        && (this->ref_count_ == other.ref_count_)
        && (this->packet_count_ == other.packet_count_)
        && (this->byte_count_ == other.byte_count_)
        && (this->duration_sec_ == other.duration_sec_)
        && (this->duration_nsec_ == other.duration_nsec_)
        && (this->bucket_stats_ == other.bucket_stats_)
        && (this->length_ == other.length_));
}

bool GroupStats::operator!=(const GroupStats &other) const {
    return !(*this == other);
}

size_t GroupStats::pack(uint8_t* buffer) {
    struct of15::ofp_group_stats *gs = (struct of15::ofp_group_stats *) buffer;
    gs->length = hton16(this->length_);
    gs->group_id = hton32(this->group_id_);
    gs->ref_count = hton32(this->ref_count_);
    gs->packet_count = hton64(this->packet_count_);
    gs->byte_count = hton64(this->byte_count_);
    gs->duration_sec = hton32(this->duration_sec_);
    gs->duration_nsec = hton32(this->duration_nsec_);
    uint8_t *p = buffer + sizeof(of15::ofp_group_stats);
    for (std::vector<BucketStats>::iterator it = this->bucket_stats_.begin();
        it != this->bucket_stats_.end(); ++it) {
        it->pack(p);
        p += sizeof(struct of15::ofp_bucket_counter);
    }
    return 0;
}

of_error GroupStats::unpack(uint8_t* buffer) {
    struct of15::ofp_group_stats *gs = (struct of15::ofp_group_stats *) buffer;
    this->length_ = ntoh16(gs->length);
    this->group_id_ = ntoh32(gs->group_id);
    this->ref_count_ = ntoh32(gs->ref_count);
    this->packet_count_ = ntoh64(gs->packet_count);
    this->byte_count_ = ntoh64(gs->byte_count);
    this->duration_sec_ = ntoh32(gs->duration_sec);
    this->duration_nsec_ = ntoh32(gs->duration_nsec);
    uint8_t *p = buffer + sizeof(of15::ofp_group_stats);
    size_t len = this->length_ - sizeof(of15::ofp_group_stats);
    while (len) {
        BucketStats stats;
        stats.unpack(p);
        this->bucket_stats_.push_back(stats);
        p += sizeof(struct of15::ofp_bucket_counter);
        len -= sizeof(struct of15::ofp_bucket_counter);
    }
    return 0;
}

void GroupStats::bucket_stats(std::vector<BucketStats> bucket_stats) {
    this->bucket_stats_ = bucket_stats;
    this->length_ += bucket_stats.size()
        * sizeof(struct of15::ofp_bucket_counter);
}
void GroupStats::add_bucket_stat(BucketStats stat) {
    this->bucket_stats_.push_back(stat);
    this->length_ += sizeof(struct of15::ofp_bucket_counter);
}

GroupDesc::GroupDesc(uint8_t type, uint32_t group_id) {
    this->type_ = type;
    this->group_id_ = group_id;
    this->length_ = sizeof(struct of15::ofp_group_desc_stats);
}

GroupDesc::GroupDesc(uint8_t type, uint32_t group_id,
    std::vector<of15::Bucket> buckets) {
    this->type_ = type;
    this->group_id_ = group_id;
    this->buckets_ = buckets;
    this->length_ = sizeof(struct of15::ofp_group_desc_stats) + buckets_len();
}

bool GroupDesc::operator==(const GroupDesc &other) const {
    return ((this->type_ == other.type_) && (this->group_id_ == other.group_id_)
        && (this->length_ == other.length_)
        && (this->buckets_ == other.buckets_));
}

bool GroupDesc::operator!=(const GroupDesc &other) const {
    return !(*this == other);
}

size_t GroupDesc::pack(uint8_t* buffer) {
    struct of15::ofp_group_desc_stats * gd =
        (struct of15::ofp_group_desc_stats *) buffer;
    gd->length = hton16(this->length_);
    gd->type = this->type_;
    memset(&gd->pad, 0x0, 1);
    gd->group_id = hton32(this->group_id_);
    uint8_t *p = buffer + sizeof(struct of15::ofp_group_desc_stats);
    for (std::vector<Bucket>::iterator it = this->buckets_.begin();
        it != this->buckets_.end(); ++it) {
        it->pack(p);
        p += it->len();
    }
    return this->length_;
}

of_error GroupDesc::unpack(uint8_t* buffer) {
    struct of15::ofp_group_desc_stats * gd =
        (struct of15::ofp_group_desc_stats *) buffer;
    this->length_ = ntoh16(gd->length);
    this->type_ = gd->type;
    this->group_id_ = ntoh32(gd->group_id);
    size_t len = this->length_ - sizeof(struct of15::ofp_group_desc_stats);
    uint8_t *p = buffer + sizeof(struct of15::ofp_group_desc_stats);
    while (len) {
        Bucket bucket;
        bucket.unpack(p);
        this->buckets_.push_back(bucket);
        p += bucket.len();
        len -= bucket.len();
    }
    return 0;
}

void GroupDesc::buckets(std::vector<Bucket> buckets) {
    this->buckets_ = buckets;
    this->length_ += buckets_len();
}

void GroupDesc::add_bucket(Bucket bucket) {
    this->buckets_.push_back(bucket);
    this->length_ += bucket.len();
}

size_t GroupDesc::buckets_len() {
    size_t len = 0;
    for (std::vector<Bucket>::iterator it = this->buckets_.begin();
        it != this->buckets_.end(); ++it) {
        len += it->len();
    }
    return len;
}
;

GroupFeatures::GroupFeatures(uint32_t types, uint32_t capabilities,
    uint32_t max_groups[4], uint32_t actions[4]) {
    this->types_ = types;
    this->capabilities_ = capabilities;
    memcpy(this->max_groups_, max_groups, 16);
    memcpy(this->actions_, actions, 16);
}

bool GroupFeatures::operator==(const GroupFeatures &other) const {
    for (int i = 0; i < 4; i++) {
        if (this->max_groups_[i] != other.max_groups_[i]) {
            return false;
        }
        if (this->actions_[i] != other.actions_[i]) {
            return false;
        }
    }
    return ((this->types_ == other.types_)
        && (this->capabilities_ == other.capabilities_));
}

bool GroupFeatures::operator!=(const GroupFeatures &other) const {
    return !(*this == other);
}

size_t GroupFeatures::pack(uint8_t* buffer) {
    struct of15::ofp_group_features *gf =
        (struct of15::ofp_group_features*) buffer;
    gf->types = hton32(this->types_);
    gf->capabilities = hton32(this->capabilities_);
    for (int i = 0; i < 4; i++) {
        gf->max_groups[i] = hton32(this->max_groups_[i]);
        gf->actions[i] = hton32(this->actions_[i]);
    }
    return 0;
}

of_error GroupFeatures::unpack(uint8_t* buffer) {
    struct of15::ofp_group_features *gf =
        (struct of15::ofp_group_features*) buffer;
    this->types_ = ntoh32(gf->types);
    this->capabilities_ = ntoh32(gf->capabilities);
    for (int i = 0; i < 4; i++) {
        this->max_groups_[i] = ntoh32(gf->max_groups[i]);
        this->actions_[i] = ntoh32(gf->actions[i]);
    }
    return 0;
}

TableFeatureProp::TableFeatureProp(uint16_t type) {
    this->type_ = type;
    this->length_ = sizeof(struct of15::ofp_table_feature_prop_header);
}

bool TableFeatureProp::equals(const TableFeatureProp &other) {
    return ((*this == other));
}

bool TableFeatureProp::operator==(const TableFeatureProp &other) const {
    return ((this->type_ == other.type_) && (this->length_ == other.length_));
}

bool TableFeatureProp::operator!=(const TableFeatureProp &other) const {
    return !(*this == other);
}

size_t TableFeatureProp::pack(uint8_t* buffer) {
    struct of15::ofp_table_feature_prop_header *fp =
        (struct of15::ofp_table_feature_prop_header*) buffer;
    fp->type = hton16(this->type_);
    fp->length = hton16(this->length_);
    return 0;
}

of_error TableFeatureProp::unpack(uint8_t* buffer) {
    struct of15::ofp_table_feature_prop_header *fp =
        (struct of15::ofp_table_feature_prop_header*) buffer;
    this->type_ = ntoh16(fp->type);
    this->length_ = ntoh16(fp->length);
    return 0;
}

//Aman
TableFeaturePropInstruction::TableFeaturePropInstruction(uint16_t type,
    std::vector<Instruction> instruction_ids)
    : TableFeatureProp(type) {
    this->instruction_ids_ = instruction_ids;
    this->length_ += instruction_ids.size()
        * sizeof(struct of15::ofp_instruction_id);
}

bool TableFeaturePropInstruction::equals(const TableFeatureProp &other) {

    if (const TableFeaturePropInstruction * prop =
        dynamic_cast<const TableFeaturePropInstruction *>(&other)) {
        return ((TableFeatureProp::equals(other))
            && (this->instruction_ids_ == prop->instruction_ids_));
    }
    else {
        return false;
    }
}

size_t TableFeaturePropInstruction::pack(uint8_t* buffer) {
    TableFeatureProp::pack(buffer);
    uint8_t *p = buffer + sizeof(struct of15::ofp_table_feature_prop_header);
    for (std::vector<Instruction>::iterator it = this->instruction_ids_.begin();
        it != this->instruction_ids_.end(); ++it) {
        it->pack(p);
        p += sizeof(struct of15::ofp_instruction_id);
    }
    return 0;
}

of_error TableFeaturePropInstruction::unpack(uint8_t* buffer) {
    TableFeatureProp::unpack(buffer);
    uint8_t *p = buffer + sizeof(struct of15::ofp_table_feature_prop_header);
    size_t len = this->length_
        - sizeof(struct of15::ofp_table_feature_prop_header);
    Instruction inst;
    while (len) {
        inst.unpack(p);
        this->instruction_ids_.push_back(inst);
        p += sizeof(struct of15::ofp_instruction_id);
        len -= sizeof(struct of15::ofp_instruction_id);
    }
    return 0;
}

void TableFeaturePropInstruction::instruction_ids(
    std::vector<Instruction> instruction_ids) {
    this->instruction_ids_ = instruction_ids;
    //Total length with padding
    this->length_ += instruction_ids.size()
        * sizeof(struct of15::ofp_instruction_id);
}

TableFeaturePropNextTables::TableFeaturePropNextTables(uint16_t type,
    std::vector<uint8_t> next_table_ids)
    : TableFeatureProp(type) {
    this->next_table_ids_ = next_table_ids_;
    this->length_ += next_table_ids_.size();
}

bool TableFeaturePropNextTables::equals(const TableFeatureProp &other) {

    if (const TableFeaturePropNextTables * prop =
        dynamic_cast<const TableFeaturePropNextTables *>(&other)) {
        return ((TableFeatureProp::equals(other))
            && (this->next_table_ids_ == prop->next_table_ids_));
    }
    else {
        return false;
    }
}

size_t TableFeaturePropNextTables::pack(uint8_t* buffer) {
    TableFeatureProp::pack(buffer);
    uint8_t *p = buffer + sizeof(struct of15::ofp_table_feature_prop_header);
    for (std::vector<uint8_t>::iterator it = this->next_table_ids_.begin();
        it != this->next_table_ids_.end(); ++it) {
        memcpy(p, &(*it), sizeof(uint8_t));
        p += sizeof(uint8_t);
    }
    return 0;
}

of_error TableFeaturePropNextTables::unpack(uint8_t* buffer) {
    TableFeatureProp::unpack(buffer);
    uint8_t *p = buffer + sizeof(struct of15::ofp_table_feature_prop_header);
    size_t len = this->length_
        - sizeof(struct of15::ofp_table_feature_prop_header);
    while (len) {
        this->next_table_ids_.push_back(*p);
        p += sizeof(uint8_t);
        len -= sizeof(uint8_t);
    }
    return 0;
}

void TableFeaturePropNextTables::table_ids(
    std::vector<uint8_t> next_table_ids) {
    this->next_table_ids_ = next_table_ids;
    this->length_ += next_table_ids_.size() * sizeof(uint8_t);
}

TableFeaturePropActions::TableFeaturePropActions(uint16_t type,
    std::vector<Action> action_ids)
    : TableFeatureProp(type) {
    this->action_ids_ = action_ids;
    this->length_ += action_ids.size() * sizeof(struct ofp_action_header);
}

bool TableFeaturePropActions::equals(const TableFeatureProp &other) {

    if (const TableFeaturePropActions * prop =
        dynamic_cast<const TableFeaturePropActions *>(&other)) {
        return ((TableFeatureProp::equals(other))
            && (this->action_ids_ == prop->action_ids_));
    }
    else {
        return false;
    }
}

size_t TableFeaturePropActions::pack(uint8_t* buffer) {
    TableFeatureProp::pack(buffer);
    uint8_t *p = buffer + sizeof(struct of15::ofp_table_feature_prop_header);
    for (std::vector<Action>::iterator it = this->action_ids_.begin();
        it != this->action_ids_.end(); ++it) {
        it->pack(p);
        p += sizeof(struct ofp_action_header);
    }
    return 0;
}

of_error TableFeaturePropActions::unpack(uint8_t* buffer) {
    TableFeatureProp::unpack(buffer);
    uint8_t *p = buffer + sizeof(struct of15::ofp_table_feature_prop_header);
    size_t len = this->length_
        - sizeof(struct of15::ofp_table_feature_prop_header);
    Action act;
    while (len) {
        act.unpack(p);
        this->action_ids_.push_back(act);
        p += sizeof(struct ofp_action_header);
        len -= sizeof(struct ofp_action_header);
    }
    return 0;
}

void TableFeaturePropActions::action_ids(std::vector<Action> action_ids) {
    this->action_ids_ = action_ids;
    this->length_ += action_ids.size() * sizeof(struct ofp_action_header);
}

TableFeaturePropOXM::TableFeaturePropOXM(uint16_t type,
    std::vector<uint32_t> oxm_ids)
    : TableFeatureProp(type) {
    this->oxm_ids_ = oxm_ids;
    this->length_ += oxm_ids_.size() * sizeof(uint32_t);
}

bool TableFeaturePropOXM::equals(const TableFeatureProp &other) {
    if (const TableFeaturePropOXM * prop =
        dynamic_cast<const TableFeaturePropOXM *>(&other)) {
        return ((TableFeatureProp::equals(other))
            && (this->oxm_ids_ == prop->oxm_ids_));
    }
    else {
        return false;
    }
}

size_t TableFeaturePropOXM::pack(uint8_t* buffer) {
    TableFeatureProp::pack(buffer);
    uint8_t *p = buffer + sizeof(struct of15::ofp_table_feature_prop_header);
    for (std::vector<uint32_t>::iterator it = this->oxm_ids_.begin();
        it != this->oxm_ids_.end(); ++it) {
        memcpy(p, &(*it), sizeof(uint32_t));
        p += sizeof(uint32_t);
    }
    return 0;
}

of_error TableFeaturePropOXM::unpack(uint8_t* buffer) {
    TableFeatureProp::unpack(buffer);
    uint8_t *p = buffer + sizeof(struct of15::ofp_table_feature_prop_header);
    size_t len = this->length_
        - sizeof(struct of15::ofp_table_feature_prop_header);
    while (len) {
        uint32_t *oxm_id = (uint32_t*) p;
        this->oxm_ids_.push_back(*oxm_id);
        p += sizeof(uint32_t);
        len -= sizeof(uint32_t);
    }
    return 0;
}

void TableFeaturePropOXM::oxm_ids(std::vector<uint32_t> oxm_ids) {
    this->oxm_ids_ = oxm_ids;
    this->length_ += oxm_ids_.size() * sizeof(uint32_t);
}

TableFeaturePropExperimenter::TableFeaturePropExperimenter(uint16_t type,
    uint32_t experimenter, uint32_t exp_type)
    : TableFeatureProp(type) {
    this->experimenter_ = experimenter;
    this->exp_type_ = exp_type;
    this->length_ += sizeof(struct of15::ofp_table_feature_prop_experimenter);
}

bool TableFeaturePropExperimenter::equals(const TableFeatureProp &other) {

    if (const TableFeaturePropExperimenter * prop =
        dynamic_cast<const TableFeaturePropExperimenter *>(&other)) {
        return ((TableFeatureProp::equals(other))
            && (this->experimenter_ == prop->experimenter_)
            && (this->exp_type_ == prop->exp_type_));
    }
    else {
        return false;
    }
}

size_t TableFeaturePropExperimenter::pack(uint8_t* buffer) {
    struct of15::ofp_table_feature_prop_experimenter *pe =
        (struct of15::ofp_table_feature_prop_experimenter*) buffer;
    TableFeatureProp::pack(buffer);
    pe->experimenter = hton32(this->experimenter_);
    pe->exp_type = hton32(this->exp_type_);
    return 0;
}

of_error TableFeaturePropExperimenter::unpack(uint8_t* buffer) {
    struct of15::ofp_table_feature_prop_experimenter *pe =
        (struct of15::ofp_table_feature_prop_experimenter*) buffer;
    TableFeatureProp::unpack(buffer);
    this->experimenter_ = ntoh32(pe->experimenter);
    this->exp_type_ = ntoh32(pe->exp_type);
    return 0;
}

TablePropertiesList::TablePropertiesList(
    std::list<TableFeatureProp*> property_list) {
    this->property_list_ = property_list_;
    for (std::list<TableFeatureProp*>::const_iterator it =
        property_list.begin(); it != property_list.end(); ++it) {
        this->length_ += (*it)->length();
    }
}

TablePropertiesList::TablePropertiesList(const TablePropertiesList &other) {
    this->length_ = other.length_;
    for (std::list<TableFeatureProp*>::const_iterator it =
        other.property_list_.begin(); it != other.property_list_.end(); ++it) {
        this->property_list_.push_back((*it)->clone());
    }
}

TablePropertiesList::~TablePropertiesList() {
    this->property_list_.remove_if(TableFeatureProp::delete_all);
}

bool TablePropertiesList::operator==(const TablePropertiesList &other) const {
    std::list<TableFeatureProp*>::const_iterator ot =
        other.property_list_.begin();
    for (std::list<TableFeatureProp*>::const_iterator it =
        this->property_list_.begin(); it != this->property_list_.end();
        ++it, ++ot) {
        if (!((*it)->equals(**ot))) {
            return false;
        }
    }
    return true;
}

bool TablePropertiesList::operator!=(const TablePropertiesList &other) const {
    return !(*this == other);
}

size_t TablePropertiesList::pack(uint8_t* buffer) {
    uint8_t *p = buffer;
    for (std::list<TableFeatureProp*>::iterator it =
        this->property_list_.begin(), end = this->property_list_.end();
        it != end; it++) {
        (*it)->pack(p);
        p += (*it)->length();
    }
    return 0;
}

of_error TablePropertiesList::unpack(uint8_t* buffer) {
    uint8_t *p = buffer;
    size_t len = this->length_;
    TableFeatureProp *prop;
    while (len) {
        uint16_t type = ntoh16(*((uint16_t*) p));
        prop = TableFeatures::make_table_feature_prop(type);
        prop->unpack(p);
        this->property_list_.push_back(prop);
        len -= prop->length();
        p += prop->length();
    }
    return 0;
}

TablePropertiesList& TablePropertiesList::operator=(TablePropertiesList other) {
    swap(*this, other);
    return *this;
}

void swap(TablePropertiesList& first, TablePropertiesList& second) {
    std::swap(first.length_, second.length_);
    std::swap(first.property_list_, second.property_list_);
}

void TablePropertiesList::property_list(
    std::list<TableFeatureProp*> property_list) {
    this->property_list_ = property_list;
    for (std::list<TableFeatureProp*>::const_iterator it =
        property_list.begin(); it != property_list.end(); ++it) {
        this->length_ += (*it)->length();
    }
}

void TablePropertiesList::add_property(TableFeatureProp* prop) {
    this->property_list_.push_back(prop);
    this->length_ += prop->length();
}

TableFeatures::TableFeatures(uint8_t table_id, uint8_t command, std::string name,
    uint32_t features, uint64_t metadata_match, uint64_t metadata_write,
    uint32_t capabilities, uint32_t max_entries)
    : table_id_(table_id),
      command_(command),
      name_(name),
      features_(features),
      metadata_match_(metadata_match),
      metadata_write_(metadata_write),
      capabilities_(capabilities),
      max_entries_(max_entries) {
    this->length_ = sizeof(struct of15::ofp_table_features);
}

bool TableFeatures::operator==(const TableFeatures &other) const {
    return ((this->length_ == other.length_)
        && (this->table_id_ == other.table_id_)
        && (this->command_ == other.command_)
        && (this->name_ == other.name_)
        && (this->features_ == other.features_)
        && (this->metadata_match_ == other.metadata_match_)
        && (this->metadata_write_ == other.metadata_write_)
        && (this->capabilities_ == other.capabilities_)
        && (this->max_entries_ == other.max_entries_)
        && (this->properties_ == other.properties_));
}

bool TableFeatures::operator!=(const TableFeatures &other) const {
    return !(*this == other);
}

uint16_t TableFeatures::length() {
    return this->length_;
}

size_t TableFeatures::pack(uint8_t* buffer) {
    struct of15::ofp_table_features *tf =
        (struct of15::ofp_table_features*) buffer;
    tf->length = hton16(length());
    tf->table_id = this->table_id_;
    tf->command = this->command_;
//    memset(tf->pad, 0x0, 5); //Aman
    memset(tf->name, 0x0, OFP_MAX_TABLE_NAME_LEN);
    memcpy(tf->name, this->name_.c_str(),
        this->name_.size() < OFP_MAX_TABLE_NAME_LEN ?
            this->name_.size() : OFP_MAX_TABLE_NAME_LEN);
    tf->features = hton32(this->features_); //Aman
    tf->metadata_match = hton64(this->metadata_match_);
    tf->metadata_write = hton64(this->metadata_write_);
    tf->capabilities = hton32(this->capabilities_); //Aman
    tf->max_entries = hton32(this->max_entries_);
    uint8_t *p = buffer + sizeof(struct of15::ofp_table_features);
    this->properties_.pack(p);
    return length();
}

of_error TableFeatures::unpack(uint8_t* buffer) {
    struct of15::ofp_table_features *tf =
        (struct of15::ofp_table_features*) buffer;
    this->length_ = ntoh16(tf->length);
    this->table_id_ = tf->table_id;
    this->command_ = tf->command;
    this->name_ = std::string(tf->name);
    this->features_ = tf->features;
    this->metadata_match_ = ntoh64(tf->metadata_match);
    this->metadata_write_ = ntoh64(tf->metadata_write);
    this->capabilities_ = ntoh32(tf->capabilities); //Aman
    this->max_entries_ = ntoh32(tf->max_entries);
    uint8_t *p = buffer + sizeof(struct of15::ofp_table_features);
    this->properties_.length(
        this->length_ - sizeof(struct of15::ofp_table_features));
    this->properties_.unpack(p);
    return 0;
}

void TableFeatures::properties(TablePropertiesList properties) {
    this->properties_ = properties;
    this->length_ += properties.length();

}

void TableFeatures::add_table_prop(TableFeatureProp* prop) {
    this->properties_.add_property(prop);
    this->length_ += prop->length();
}

TableFeatureProp* TableFeatures::make_table_feature_prop(uint16_t type) {
    if (type == OFPTFPT_INSTRUCTIONS || type == OFPTFPT_INSTRUCTIONS_MISS) {
        return new TableFeaturePropInstruction(type);
    }
    if (type == OFPTFPT_NEXT_TABLES || type == OFPTFPT_NEXT_TABLES_MISS) {
        return new TableFeaturePropNextTables(type);
    }
    if (type == OFPTFPT_WRITE_ACTIONS || type == OFPTFPT_WRITE_ACTIONS_MISS
        || type == OFPTFPT_APPLY_ACTIONS
        || type == OFPTFPT_APPLY_ACTIONS_MISS) {
        return new TableFeaturePropActions(type);
    }
    if (type == OFPTFPT_MATCH || type == OFPTFPT_WILDCARDS
        || type == OFPTFPT_WRITE_SETFIELD || type == OFPTFPT_WRITE_SETFIELD_MISS
        || type == OFPTFPT_APPLY_SETFIELD
        || type == OFPTFPT_APPLY_SETFIELD_MISS) {
        return new TableFeaturePropOXM(type);
    }
    if (type == OFPTFPT_EXPERIMENTER || type == OFPTFPT_EXPERIMENTER_MISS) {
        return new TableFeaturePropExperimenter(type);
    }
    return NULL;
}

} //End of namespace fluid_msg

QueueProperty* QueueProperty::make_queue_of15_property(uint16_t property) {
    switch (property) {
        case (of15::OFPQDPT_MAX_RATE): { //Aman
//            return new of15::QueuePropMaxRate(); //Aman TODO: Fix this
        }
        case (of15::OFPQDPT_MIN_RATE): { //Aman
//            return new of15::QueuePropMinRate(); //Aman TODO: Fix this
        }
        case (of15::OFPQDPT_EXPERIMENTER): { //Aman TODO: Handle other enum value
            return new of15::QueueExperimenter();
        }
    }
    return NULL;
}

} //End namespace of15
