#include "of15msg.hh"
#include <iostream>

namespace fluid_msg {

//Aman
namespace of15 {

RoleCommon::RoleCommon(uint8_t version, uint8_t type)
    : OFMsg(version, type),
      role_(0),
      generation_id_(0) {
}

RoleCommon::RoleCommon(uint8_t version, uint8_t type, uint32_t xid,
    uint32_t role, uint64_t generation_id)
    : OFMsg(version, type, xid),
      role_(role),
      generation_id_(generation_id) {
    this->length_ = sizeof(struct ofp_role_request);
}

bool RoleCommon::operator==(const RoleCommon &other) const {
    return ((OFMsg::operator==(other)) && (this->role_ == other.role_)
        && (this->generation_id_ == other.generation_id_)
        && (this->length_ == other.length_));
}

bool RoleCommon::operator!=(const RoleCommon &other) const {
    return !(*this == other);
}

uint8_t* RoleCommon::pack() {
    uint8_t* buffer = OFMsg::pack();
    struct ofp_role_request * rq = (struct ofp_role_request*) buffer;
    rq->role = hton32(this->role_);
    memset(rq->pad, 0x0, 4);
    rq->generation_id = hton64(this->generation_id_);
    return buffer;
}

of_error RoleCommon::unpack(uint8_t *buffer) {
    struct ofp_role_request * rq = (struct ofp_role_request*) buffer;
    OFMsg::unpack(buffer);
    if (this->length_ < sizeof(struct ofp_role_request)) {
        return openflow_error(of15::OFPET_BAD_REQUEST, of15::OFPBRC_BAD_LEN);
    }
    this->role_ = ntoh32(rq->role);
    memset(rq->pad, 0x0, 4);
    this->generation_id_ = ntoh64(rq->generation_id);
    return 0;
}

AsyncConfigCommon::AsyncConfigCommon(uint8_t version, uint8_t type)
    : OFMsg(version, type),
      packet_in_mask_(2, 0),
      port_status_mask_(2, 0),
      flow_removed_mask_(2, 0) {
}

AsyncConfigCommon::AsyncConfigCommon(uint8_t version, uint8_t type,
    uint32_t xid)
    : OFMsg(version, type, xid),
      packet_in_mask_(2, 0),
      port_status_mask_(2, 0),
      flow_removed_mask_(2, 0) {
    this->length_ = sizeof(struct ofp_async_config);
}
;

AsyncConfigCommon::AsyncConfigCommon(uint8_t version, uint8_t type,
    uint32_t xid, std::vector<uint32_t> packet_in_mask,
    std::vector<uint32_t> port_status_mask,
    std::vector<uint32_t> flow_removed_mask)
    : OFMsg(version, type, xid),
      packet_in_mask_(packet_in_mask),
      port_status_mask_(port_status_mask),
      flow_removed_mask_(flow_removed_mask) {
    this->length_ = sizeof(struct ofp_async_config);
}

bool AsyncConfigCommon::operator==(const AsyncConfigCommon &other) const {
    return ((OFMsg::operator==(other))
        && (this->packet_in_mask_ == other.packet_in_mask_)
        && (this->port_status_mask_ == other.port_status_mask_)
        && (this->flow_removed_mask_ == other.flow_removed_mask_));
}

bool AsyncConfigCommon::operator!=(const AsyncConfigCommon &other) const {
    return !(*this == other);
}

uint8_t* AsyncConfigCommon::pack() {
    uint8_t* buffer = OFMsg::pack();
    struct ofp_async_config *ar = (struct ofp_async_config *) buffer;
//Aman
//    ar->packet_in_mask[0] = hton32(this->packet_in_mask_[0]);
//    ar->packet_in_mask[1] = hton32(this->packet_in_mask_[1]);
//    ar->port_status_mask[0] = hton32(this->port_status_mask_[0]);
//    ar->port_status_mask[1] = hton32(this->port_status_mask_[1]);
//    ar->flow_removed_mask[0] = hton32(this->flow_removed_mask_[0]);
//    ar->flow_removed_mask[1] = hton32(this->flow_removed_mask_[1]);
    return buffer;
}

of_error AsyncConfigCommon::unpack(uint8_t *buffer) {
    struct ofp_async_config *ar = (struct ofp_async_config *) buffer;
    OFMsg::unpack(buffer);
    if (this->length_ < sizeof(struct ofp_async_config)) {
        return openflow_error(of15::OFPET_BAD_REQUEST, of15::OFPBRC_BAD_LEN);
    }
//Aman
//    this->packet_in_mask_[0] = ntoh32(ar->packet_in_mask[0]);
//    this->packet_in_mask_[1] = ntoh32(ar->packet_in_mask[1]);
//    this->port_status_mask_[0] = ntoh32(ar->port_status_mask[0]);
//    this->port_status_mask_[1] = ntoh32(ar->port_status_mask[1]);
//    this->flow_removed_mask_[0] = ntoh32(ar->flow_removed_mask[0]);
//    this->flow_removed_mask_[1] = ntoh32(ar->flow_removed_mask[1]);
    return 0;
}

Hello::Hello()
    : OFMsg(of15::OFP_VERSION, of15::OFPT_HELLO) {
}

Hello::Hello(uint32_t xid)
    : OFMsg(of15::OFP_VERSION, of15::OFPT_HELLO, xid) {
}

Hello::Hello(uint32_t xid, std::list<HelloElemVersionBitmap> elements)
    : OFMsg(of15::OFP_VERSION, of15::OFPT_HELLO, xid),
      elements_(elements) {
    this->length_ += elements_len();
}

bool Hello::operator==(const Hello &other) const {
    return ((OFMsg::operator==(other)) && (this->elements_ == other.elements_));
}

bool Hello::operator!=(const Hello &other) const {
    return !(*this == other);
}

uint8_t* Hello::pack() {
    uint8_t* buffer = OFMsg::pack();
    uint8_t *p = buffer + sizeof(struct ofp_header);
    for (std::list<HelloElemVersionBitmap>::iterator it =
        this->elements_.begin(), end = this->elements_.end(); it != end; ++it) {
        it->pack(p);
        p += it->length();
    }
    return buffer;
}

of_error Hello::unpack(uint8_t* buffer) {
    OFMsg::unpack(buffer);
    /*Unpack the Hello elements*/
    uint32_t len = this->length_ - sizeof(struct ofp_header);
    uint8_t *p = buffer + sizeof(struct ofp_header);
    while (len) {
        HelloElemVersionBitmap he;
        he.unpack(p);
        len -= he.length();
        this->elements_.push_back(he);
        p += he.length();
    }
    return 0;
}

void Hello::elements(std::list<HelloElemVersionBitmap> elements) {
    this->elements_ = elements;
    this->length_ += elements_len();
}

void Hello::add_element(HelloElemVersionBitmap element) {
    this->elements_.push_back(element);
    this->length_ += element.length();

}

uint32_t Hello::elements_len() {
    uint32_t len = 0;
    for (std::list<HelloElemVersionBitmap>::iterator it =
        this->elements_.begin(), end = this->elements_.end(); it != end; ++it) {
        len += (*it).length();
    }
    return len;
}

Error::Error()
    : ErrorCommon(of15::OFP_VERSION, of15::OFPT_ERROR) {
}

Error::Error(uint32_t xid, uint16_t err_type, uint16_t code)
    : ErrorCommon(of15::OFP_VERSION, of15::OFPT_ERROR, xid, err_type, code) {
}

Error::Error(uint32_t xid, uint16_t err_type, uint16_t code, uint8_t *data,
    size_t data_len)
    : ErrorCommon(of15::OFP_VERSION, of15::OFPT_ERROR, xid, err_type, code,
        data, data_len) {
}

EchoRequest::EchoRequest()
    : EchoCommon(of15::OFP_VERSION, of15::OFPT_ECHO_REQUEST) {
}

EchoRequest::EchoRequest(uint32_t xid)
    : EchoCommon(of15::OFP_VERSION, of15::OFPT_ECHO_REQUEST, xid) {
}

EchoReply::EchoReply()
    : EchoCommon(of15::OFP_VERSION, of15::OFPT_ECHO_REPLY) {
}

EchoReply::EchoReply(uint32_t xid)
    : EchoCommon(of15::OFP_VERSION, of15::OFPT_ECHO_REPLY, xid) {
}

//Aman
//Experimenter::Experimenter()
//    : OFMsg(of15::OFP_VERSION, of15::OFPT_EXPERIMENTER) {
//    this->length_ += sizeof(struct of15::ofp_experimenter_header);
//}
//
//Experimenter::Experimenter(uint32_t xid, uint32_t experimenter,
//    uint32_t exp_type)
//    : OFMsg(of15::OFP_VERSION, of15::OFPT_EXPERIMENTER, xid),
//      experimenter_(experimenter),
//      exp_type_(exp_type) {
//    this->length_ += sizeof(struct of15::ofp_experimenter_header);
//}
//
//bool Experimenter::operator==(const Experimenter &other) const {
//    return ((Experimenter::operator==(other))
//        && (this->experimenter_ == other.experimenter_)
//        && (this->exp_type_ == other.exp_type_));
//}
//
//bool Experimenter::operator!=(const Experimenter &other) const {
//    return !(*this == other);
//}
//
//uint8_t* Experimenter::pack() {
//    uint8_t* buffer = OFMsg::pack();
//    struct of15::ofp_experimenter_header *em =
//        (struct of15::ofp_experimenter_header*) buffer;
//    em->experimenter = hton32(this->experimenter_);
//    em->exp_type = hton32(this->exp_type_);
//    return buffer;
//}
//
//of_error Experimenter::unpack(uint8_t *buffer) {
//    struct of15::ofp_experimenter_header *em =
//        (struct of15::ofp_experimenter_header*) buffer;
//    OFMsg::unpack(buffer);
//    this->experimenter_ = ntoh32(em->experimenter);
//    this->exp_type_ = ntoh32(em->exp_type);
//    return 0;
//}

FeaturesRequest::FeaturesRequest()
    : OFMsg(of15::OFP_VERSION, of15::OFPT_FEATURES_REQUEST) {
}

FeaturesRequest::FeaturesRequest(uint32_t xid)
    : OFMsg(of15::OFP_VERSION, of15::OFPT_FEATURES_REQUEST, xid) {
}

uint8_t* FeaturesRequest::pack() {
    uint8_t* buffer = OFMsg::pack();
    return buffer;
}

of_error FeaturesRequest::unpack(uint8_t *buffer) {
    OFMsg::unpack(buffer);
    if (this->length_ < sizeof(ofp_header)) {
        return openflow_error(of15::OFPET_BAD_REQUEST, of15::OFPBRC_BAD_LEN);
    }
    return 0;
}

FeaturesReply::FeaturesReply()
    : FeaturesReplyCommon(of15::OFP_VERSION, of15::OFPT_FEATURES_REPLY) {
    this->length_ = sizeof(struct of15::ofp_switch_features);
}

FeaturesReply::FeaturesReply(uint32_t xid, uint64_t datapath_id,
    uint32_t n_buffers, uint8_t n_tables, uint8_t auxiliary_id,
    uint32_t capabilities)
    : FeaturesReplyCommon(of15::OFP_VERSION, of15::OFPT_FEATURES_REPLY, xid,
          datapath_id, n_buffers, n_tables, capabilities),
      auxiliary_id_(auxiliary_id) {
    this->length_ = sizeof(struct of15::ofp_switch_features);
}

bool FeaturesReply::operator==(const FeaturesReply &other) const {
    return ((FeaturesReplyCommon::operator==(other))
        && (this->auxiliary_id_ == other.auxiliary_id_));
}

bool FeaturesReply::operator!=(const FeaturesReply &other) const {
    return !(*this == other);
}

uint8_t* FeaturesReply::pack() {
    uint8_t* buffer = OFMsg::pack();
    struct of15::ofp_switch_features *fr =
        (struct of15::ofp_switch_features *) buffer;
    fr->datapath_id = hton64(this->datapath_id_);
    fr->n_buffers = hton32(this->n_buffers_);
    fr->n_tables = this->n_tables_;
    memset(fr->pad, 0x0, 2);
    fr->auxiliary_id = this->auxiliary_id_;
    fr->capabilities = hton32(this->capabilities_);
    fr->reserved = 0;
    return buffer;
}

of_error FeaturesReply::unpack(uint8_t *buffer) {
    struct of15::ofp_switch_features *fr =
        (struct of15::ofp_switch_features *) buffer;
    OFMsg::unpack(buffer);
    if (this->length_ < sizeof(struct of15::ofp_switch_features)) {
        return openflow_error(of15::OFPET_BAD_REQUEST, of15::OFPBRC_BAD_LEN);
    }
    this->datapath_id_ = ntoh64(fr->datapath_id);
    this->n_buffers_ = ntoh32(fr->n_buffers);
    this->n_tables_ = fr->n_tables;
    memset(fr->pad, 0x0, 2);
    this->auxiliary_id_ = fr->auxiliary_id;
    this->capabilities_ = ntoh32(fr->capabilities);
    return 0;
}

GetConfigRequest::GetConfigRequest()
    : OFMsg(of15::OFP_VERSION, of15::OFPT_GET_CONFIG_REQUEST) {
}

GetConfigRequest::GetConfigRequest(uint32_t xid)
    : OFMsg(of15::OFP_VERSION, of15::OFPT_GET_CONFIG_REQUEST, xid) {
}

uint8_t* GetConfigRequest::pack() {
    uint8_t* buffer = OFMsg::pack();
    return buffer;
}

of_error GetConfigRequest::unpack(uint8_t *buffer) {
    OFMsg::unpack(buffer);
    if (this->length_ < sizeof(struct ofp_header)) {
        return openflow_error(of15::OFPET_BAD_REQUEST, of15::OFPBRC_BAD_LEN);
    }
    return 0;
}

GetConfigReply::GetConfigReply()
    : SwitchConfigCommon(of15::OFP_VERSION, of15::OFPT_GET_CONFIG_REPLY) {
}

GetConfigReply::GetConfigReply(uint32_t xid, uint16_t flags,
    uint16_t miss_send_len)
    : SwitchConfigCommon(of15::OFP_VERSION, of15::OFPT_GET_CONFIG_REPLY, xid,
        flags, miss_send_len) {
}

SetConfig::SetConfig()
    : SwitchConfigCommon(of15::OFP_VERSION, of15::OFPT_SET_CONFIG) {
}

SetConfig::SetConfig(uint32_t xid, uint16_t flags, uint16_t miss_send_len)
    : SwitchConfigCommon(of15::OFP_VERSION, of15::OFPT_SET_CONFIG, xid, flags,
        miss_send_len) {
}

PacketOut::PacketOut()
    : PacketOutCommon(of15::OFP_VERSION, of15::OFPT_PACKET_OUT) {
    this->length_ = sizeof(struct of15::ofp_packet_out);
}

PacketOut::PacketOut(uint32_t xid, uint32_t buffer_id)
    : PacketOutCommon(of15::OFP_VERSION, of15::OFPT_PACKET_OUT, xid, buffer_id) {
    this->length_ = sizeof(struct of15::ofp_packet_out);
}

bool PacketOut::operator==(const PacketOut &other) const {
    return ((PacketOutCommon::operator==(other))
        && (this->match_ == other.match_));
}

bool PacketOut::operator!=(const PacketOut &other) const {
    return !(*this == other);
}

uint8_t* PacketOut::pack() {
    size_t data_size;
    uint8_t* buffer = OFMsg::pack();
    struct of15::ofp_packet_out *po = (struct of15::ofp_packet_out*) buffer;
    po->buffer_id = hton32(this->buffer_id_);
    po->actions_len = hton16(this->actions_len_);
    memset(po->pad, 0x0, 2);
    this->match_.pack(buffer + sizeof(struct of15::ofp_packet_out) - sizeof(struct of15::ofp_match));
    this->actions_.pack(buffer + sizeof(struct of15::ofp_packet_out) - sizeof(struct of15::ofp_match) + ROUND_UP(this->match_.length(), 8));
    data_size = this->length_
        - (sizeof(struct of15::ofp_packet_out) - sizeof(struct of15::ofp_match) + ROUND_UP(this->match_.length(), 8) + this->actions_len_);
    uint8_t *p = buffer + sizeof(struct of15::ofp_packet_out)
        - sizeof(struct of15::ofp_match) + ROUND_UP(this->match_.length(), 8) + this->actions_len_;
    memcpy(p, this->data_, data_size);
    return buffer;
}

of_error PacketOut::unpack(uint8_t *buffer) {
    struct of15::ofp_packet_out *po = (struct of15::ofp_packet_out*) buffer;
    OFMsg::unpack(buffer);
    if (this->length_ < sizeof(struct of15::ofp_packet_out)) {
        return openflow_error(of15::OFPET_BAD_REQUEST, of15::OFPBRC_BAD_LEN);
    }
    this->buffer_id_ = ntoh32(po->buffer_id);
    this->actions_len_ = ntoh16(po->actions_len);
    size_t len = this->actions_len_;
    uint8_t *p = buffer
        + (sizeof(struct of15::ofp_packet_out) - sizeof(struct of15::ofp_match));
    this->match_.unpack(p);
    p += ROUND_UP(this->match_.length(), 8);
    this->actions_.length(this->actions_len_);
    this->actions_.unpack15(p);
    size_t used_up = sizeof(struct of15::ofp_packet_out) - sizeof(struct of15::ofp_match) + ROUND_UP(this->match_.length(), 8) + this->actions_len_;
    len = this->length_ - used_up;
    /*Reuse p to calculate the packet data position */
    p += this->actions_len_;
    this->data_len_ = len; //Aman
    if (len) {
        this->data_ = ::operator new(len);
        memcpy(this->data_, p, len);
    }
    return 0;
}

OXMTLV * PacketOut::get_oxm_field(uint8_t field) {
    return this->match_.oxm_field(field);
}

void PacketOut::add_oxm_field(OXMTLV &field) {
    this->match_.add_oxm_field(field);
}

void PacketOut::add_oxm_field(OXMTLV *field) {
    this->match_.add_oxm_field(field);
}

PacketIn::PacketIn()
    : PacketInCommon(of15::OFP_VERSION, of15::OFPT_PACKET_IN),
      table_id_(0),
      cookie_(0) {
}

PacketIn::PacketIn(uint32_t xid, uint32_t buffer_id, uint16_t total_len,
    uint8_t reason, uint8_t table_id, uint64_t cookie)
    : PacketInCommon(of15::OFP_VERSION, of15::OFPT_PACKET_IN, xid, buffer_id,
          total_len, reason),
      table_id_(table_id),
      cookie_(cookie) {
}

uint16_t PacketIn::length() {
    return sizeof(struct of15::ofp_packet_in) - sizeof(struct of15::ofp_match)
        + ROUND_UP(this->match_.length(), 8) + 2 + this->data_len_; //Aman (added 2 pad bytes)
}

bool PacketIn::operator==(const PacketIn &other) const {
    return ((PacketInCommon::operator==(other))
        && (this->table_id_ == other.table_id_)
        && (this->cookie_ == other.cookie_) && (this->match_ == other.match_));
}

bool PacketIn::operator!=(const PacketIn &other) const {
    return !(*this == other);
}

uint8_t* PacketIn::pack() { //Aman
    this->length_ = length();
    uint8_t* buffer = OFMsg::pack();
    size_t padding = ROUND_UP(this->match_.length(), 8) - this->match_.length();
    struct of15::ofp_packet_in *pi = (struct of15::ofp_packet_in*) buffer;
    pi->buffer_id = hton32(this->buffer_id_);
    pi->total_len = hton16(this->total_len_);
    pi->reason = this->reason_;
    pi->table_id = this->table_id_;
    pi->cookie = hton64(this->cookie_);
    uint8_t *p = buffer
        + (sizeof(struct of15::ofp_packet_in) - sizeof(struct of15::ofp_match));
    this->match_.pack(p);
    p += match_.length();
    memset(p, 0x0, padding);
    p += padding;
    memset(p, 0x0, 2);
    memcpy(p + 2, this->data_, this->data_len_);
    return buffer;
}

of_error PacketIn::unpack(uint8_t *buffer) { //Aman
    struct of15::ofp_packet_in *pi = (struct of15::ofp_packet_in*) buffer;
    OFMsg::unpack(buffer);
    this->buffer_id_ = ntoh32(pi->buffer_id);
    this->total_len_ = ntoh16(pi->total_len);
    this->reason_ = pi->reason;
    this->table_id_ = pi->table_id;
    this->cookie_ = ntoh64(pi->cookie);
    uint8_t *p = buffer
        + (sizeof(struct of15::ofp_packet_in) - sizeof(struct of15::ofp_match));
    this->match_.unpack(p);
    p += ROUND_UP(this->match_.length(), 8) + 2;
    this->data_len_ = this->length_
        - (sizeof(struct of15::ofp_packet_in) - sizeof(struct of15::ofp_match)
            + ROUND_UP(this->match_.length(), 8) + 2);
    this->data_ = ::operator new(this->data_len_);
    memcpy(this->data_, p, this->data_len_);
    return 0;
}

OXMTLV * PacketIn::get_oxm_field(uint8_t field) {
    return this->match_.oxm_field(field);
}

void PacketIn::add_oxm_field(OXMTLV &field) {
    this->match_.add_oxm_field(field);
}

void PacketIn::add_oxm_field(OXMTLV *field) {
    this->match_.add_oxm_field(field);
}

FlowMod::FlowMod()
    : FlowModCommon(of15::OFP_VERSION, of15::OFPT_FLOW_MOD),
      instructions_(),
      command_(0),
      cookie_mask_(0),
      table_id_(0),
      out_port_(0),
      out_group_(0),
      importance_(0) {
}

FlowMod::FlowMod(uint32_t xid, uint64_t cookie, uint64_t cookie_mask,
    uint8_t table_id, uint8_t command, uint16_t idle_timeout,
    uint16_t hard_timeout, uint16_t priority, uint32_t buffer_id,
    uint32_t out_port, uint32_t out_group, uint16_t flags, uint16_t importance)
    : FlowModCommon(of15::OFP_VERSION, of15::OFPT_FLOW_MOD, xid, cookie,
          idle_timeout, hard_timeout, priority, buffer_id, flags),
      instructions_(),
      command_(command),
      cookie_mask_(cookie_mask),
      table_id_(table_id),
      out_port_(out_port),
      out_group_(out_group),
      importance_(importance) {
}

uint16_t FlowMod::length() {
    return sizeof(struct of15::ofp_flow_mod) - sizeof(struct of15::ofp_match)
        + ROUND_UP(this->match_.length(), 8) + this->instructions_.length();
}

bool FlowMod::operator==(const FlowMod &other) const {
    return ((FlowModCommon::operator==(other))
        && (this->command_ == other.command_)
        && (this->cookie_mask_ == other.cookie_mask_)
        && (this->table_id_ == other.table_id_)
        && (this->out_port_ == other.out_port_)
        && (this->out_group_ == other.out_group_)
        && (this->instructions_ == other.instructions_));
}

bool FlowMod::operator!=(const FlowMod &other) const {
    return !(*this == other);
}

uint8_t* FlowMod::pack() {
    this->length_ = length();
    uint8_t* buffer = OFMsg::pack();
    size_t padding = ROUND_UP(this->match_.length(), 8) - this->match_.length();
    struct of15::ofp_flow_mod *fm = (struct of15::ofp_flow_mod*) buffer;
    fm->cookie = hton64(this->cookie_);
    fm->cookie_mask = hton64(this->cookie_mask_);
    fm->table_id = this->table_id_;
    fm->command = this->command_;
    fm->idle_timeout = hton16(this->idle_timeout_);
    fm->hard_timeout = hton16(this->hard_timeout_);
    fm->priority = hton16(this->priority_);
    fm->buffer_id = hton32(this->buffer_id_);
    fm->out_port = hton32(this->out_port_);
    fm->out_group = hton32(this->out_group_);
    fm->flags = hton16(this->flags_);
//    memset(fm->pad, 0x0, 2);//Aman
    uint8_t *p = buffer
        + (sizeof(struct of15::ofp_flow_mod) - sizeof(struct of15::ofp_match));
    this->match_.pack(p);
    p += this->match_.length();
    memset(p, 0x0, padding);
    p += padding;
    this->instructions_.pack(p);
    return buffer;
}

of_error FlowMod::unpack(uint8_t *buffer) {
    OFMsg::unpack(buffer);
    of_error err;
    struct of15::ofp_flow_mod *fm = (struct of15::ofp_flow_mod*) buffer;
    if (this->length_ < sizeof(struct of15::ofp_flow_mod)) {
        return openflow_error(of15::OFPET_BAD_REQUEST, of15::OFPBRC_BAD_LEN);
    }
    this->cookie_ = ntoh64(fm->cookie);
    this->cookie_mask_ = ntoh64(fm->cookie_mask);
    this->table_id_ = fm->table_id;
    this->command_ = fm->command;
    this->idle_timeout_ = ntoh16(fm->idle_timeout);
    this->hard_timeout_ = ntoh16(fm->hard_timeout);
    this->priority_ = ntoh16(fm->priority);
    this->buffer_id_ = ntoh32(fm->buffer_id);
    this->out_port_ = ntoh32(fm->out_port);
    this->out_group_ = ntoh32(fm->out_group);
    this->flags_ = ntoh16(fm->flags);
    this->importance_ = ntoh16(fm->importance);
    uint8_t *p = buffer
        + (sizeof(struct of15::ofp_flow_mod) - sizeof(struct of15::ofp_match));
    err = this->match_.unpack(p);
    if (err) {
        return err;
    }
    this->instructions_.length(
        this->length_
            - ((sizeof(struct of15::ofp_flow_mod)
                - sizeof(struct of15::ofp_match))
                + ROUND_UP(this->match_.length(), 8)));
    p += ROUND_UP(this->match_.length(), 8);
    this->instructions_.unpack(p);
    return 0;
}

OXMTLV * FlowMod::get_oxm_field(uint8_t field) {
    return this->match_.oxm_field(field);
}

void FlowMod::match(of15::Match match) {
    this->match_ = match;
    this->length_ += this->match_.length();
}

void FlowMod::add_oxm_field(OXMTLV &field) {
    this->match_.add_oxm_field(field);
}

void FlowMod::add_oxm_field(OXMTLV *field) {
    this->match_.add_oxm_field(field);
}

void FlowMod::instructions(InstructionSet instructions) {
    this->instructions_ = instructions;
    this->length_ += instructions.length();
}

void FlowMod::add_instruction(Instruction &inst) {
    this->instructions_.add_instruction(inst);
    this->length_ += inst.length();
}

void FlowMod::add_instruction(Instruction* inst) {
    this->instructions_.add_instruction(inst);
    this->length_ += inst->length();
}

FlowRemoved::FlowRemoved()
    : OFMsg(of15::OFP_VERSION, of15::OFPT_FLOW_REMOVED),
      table_id_(0),
      reason_(0),
      priority_(0),
      idle_timeout_(0),
      hard_timeout_(0),
      cookie_(0) {
}

FlowRemoved::FlowRemoved(uint32_t xid, uint8_t table_id, uint8_t reason, uint16_t priority,
        uint16_t idle_timeout, uint16_t hard_timeout, uint64_t cookie)
    : OFMsg(of15::OFP_VERSION, of15::OFPT_FLOW_REMOVED, xid),
      table_id_(table_id),
      reason_(reason),
      priority_(priority),
      idle_timeout_(idle_timeout),
      hard_timeout_(hard_timeout),
      cookie_(cookie) {
}

FlowRemoved::FlowRemoved(uint32_t xid, uint8_t table_id, uint8_t reason, uint16_t priority,
        uint16_t idle_timeout, uint16_t hard_timeout, uint64_t cookie, of15::Match match,
        of15::Stats stats)
    : OFMsg(of15::OFP_VERSION, of15::OFPT_FLOW_REMOVED, xid),
      table_id_(table_id),
      reason_(reason),
      priority_(priority),
      idle_timeout_(idle_timeout),
      hard_timeout_(hard_timeout),
      cookie_(cookie),
      match_(match),
      stats_(stats) {
}

uint16_t FlowRemoved::length() {
    return sizeof(struct of15::ofp_flow_removed)
        - sizeof(struct of15::ofp_match) + ROUND_UP(this->match_.length(), 8)
        + ROUND_UP(this->stats_.length(), 8);
}

bool FlowRemoved::operator==(const FlowRemoved &other) const {
    return ((this->table_id_ == other.table_id_)
        && (this->reason_ == other.reason_)
        && (this->priority_ == other.priority_)
        && (this->idle_timeout_ == other.idle_timeout_)
        && (this->hard_timeout_ == other.hard_timeout_)
        && (this->cookie_ == other.cookie_)
        && (this->match_ == other.match_)
        && (this->stats_ == other.stats_));
}

bool FlowRemoved::operator!=(const FlowRemoved &other) const {
    return !(*this == other);
}

uint8_t* FlowRemoved::pack() {
    this->length_ = length();
    uint8_t* buffer = OFMsg::pack();
    struct of15::ofp_flow_removed *fr = (struct of15::ofp_flow_removed*) buffer;
    fr->table_id = this->table_id_;
    fr->reason = this->reason_;
    fr->priority = hton16(this->priority_);
    fr->idle_timeout = hton16(this->idle_timeout_);
    fr->hard_timeout = hton16(this->hard_timeout_);
    fr->cookie = hton64(this->cookie_);
    uint8_t *p = buffer
        + (sizeof(struct of15::ofp_flow_removed)
            - sizeof(struct of15::ofp_match));
    this->match_.pack(p);
    p += this->match_.length();
    size_t padding = ROUND_UP(this->match_.length(), 8) - this->match_.length();
    memset(p, 0x0, padding);
    p += padding;
    this->stats_.pack(p);
    p += this->stats_.length();
    padding = ROUND_UP(this->stats_.length(), 8) - this->stats_.length();
    memset(p, 0x0, padding);
    return buffer;
}

of_error FlowRemoved::unpack(uint8_t *buffer) {
    struct of15::ofp_flow_removed *fr = (struct of15::ofp_flow_removed*) buffer;
    OFMsg::unpack(buffer);
    this->table_id_ = fr->table_id;
    this->reason_ = fr->reason;
    this->priority_ = ntoh16(fr->priority);
    this->idle_timeout_ = ntoh16(fr->idle_timeout);
    this->hard_timeout_ = ntoh16(fr->hard_timeout);
    this->cookie_ = ntoh64(fr->cookie);

    uint8_t *p = buffer
        + (sizeof(struct of15::ofp_flow_removed)
            - sizeof(struct of15::ofp_match));
    this->match_.unpack(p);
    p += ROUND_UP(this->match_.length(), 8);
    this->stats_.unpack(p);
    return 0;
}

PortStatus::PortStatus()
    : PortStatusCommon(of15::OFP_VERSION, of15::OFPT_PORT_STATUS) {
    this->length_ = sizeof(struct of15::ofp_port_status);
}

PortStatus::PortStatus(uint32_t xid, uint8_t reason, of15::Port desc)
    : PortStatusCommon(of15::OFP_VERSION, of15::OFPT_PORT_STATUS, xid, reason),
      desc_(desc) {
    this->length_ = sizeof(struct of15::ofp_port_status);
}

bool PortStatus::operator==(const PortStatus &other) const {
    return ((PortStatusCommon::operator==(other))
        && (this->desc_ == other.desc_));
}

bool PortStatus::operator!=(const PortStatus &other) const {
    return !(*this == other);
}

uint8_t* PortStatus::pack() {
    uint8_t* buffer = OFMsg::pack();
    struct of15::ofp_port_status *ps = (struct of15::ofp_port_status *) buffer;
    ps->reason = this->reason_;
    memset(ps->pad, 0x0, 7);
    this->desc_.pack(buffer + (sizeof(struct ofp_header) + 8));
    return buffer;
}

of_error PortStatus::unpack(uint8_t *buffer) {
    struct of15::ofp_port_status *ps = (struct of15::ofp_port_status *) buffer;
    OFMsg::unpack(buffer);
    this->reason_ = ps->reason;
    this->desc_.unpack(buffer + (sizeof(struct ofp_header) + 8));
    return 0;
}

PortMod::PortMod()
    : PortModCommon(of15::OFP_VERSION, of15::OFPT_PORT_MOD) {
    this->length_ = sizeof(struct of15::ofp_port_mod);
}

PortMod::PortMod(uint32_t xid, uint32_t port_no, EthAddress hw_addr,
    uint32_t config, uint32_t mask, uint32_t advertise)
    : PortModCommon(of15::OFP_VERSION, of15::OFPT_PORT_MOD, xid, hw_addr,
          config, mask, advertise),
      port_no_(port_no) {
    this->length_ = sizeof(struct of15::ofp_port_mod);
}

bool PortMod::operator==(const PortMod &other) const {
    return ((PortModCommon::operator==(other))
        && (this->port_no_ == other.port_no_));
}

bool PortMod::operator!=(const PortMod &other) const {
    return !(*this == other);
}

uint8_t* PortMod::pack() {
    uint8_t* buffer = OFMsg::pack();
    struct of15::ofp_port_mod *pm = (struct of15::ofp_port_mod *) buffer;
    pm->port_no = hton32(this->port_no_);
    memset(pm->pad, 0x0, 4);
    memcpy(pm->hw_addr, hw_addr_.get_data(), OFP_ETH_ALEN);
    memset(pm->pad, 0x0, 2);
    pm->config = hton32(this->config_);
    pm->mask = hton32(this->mask_);
//    pm->advertise = hton32(this->advertise_); //Aman
    memset(pm->pad, 0x0, 4);
    return buffer;
}

of_error PortMod::unpack(uint8_t* buffer) {
    struct of15::ofp_port_mod *pm = (struct of15::ofp_port_mod *) buffer;
    if (pm->header.length < sizeof(struct of15::ofp_port_mod)) {
        return openflow_error(of15::OFPET_BAD_REQUEST, of15::OFPBRC_BAD_LEN);
    }
    OFMsg::unpack(buffer);
    this->port_no_ = ntoh32(pm->port_no);
    this->hw_addr_ = EthAddress(pm->hw_addr);
    this->config_ = ntoh32(pm->config);
    this->mask_ = ntoh32(pm->mask);
//    this->advertise_ = ntoh32(pm->advertise); //Aman
    return 0;
}

GroupMod::GroupMod()
    : OFMsg(of15::OFP_VERSION, of15::OFPT_GROUP_MOD) {
    this->length_ = sizeof(struct of15::ofp_group_mod);
}

GroupMod::GroupMod(uint32_t xid, uint16_t command, uint8_t type,
    uint32_t group_id)
    : OFMsg(of15::OFP_VERSION, of15::OFPT_GROUP_MOD, xid),
      command_(command),
      group_type_(type),
      group_id_(group_id) {
    this->length_ = sizeof(struct of15::ofp_group_mod);
}

GroupMod::GroupMod(uint32_t xid, uint16_t command, uint8_t type,
    uint32_t group_id, std::vector<Bucket> buckets)
    : OFMsg(of15::OFP_VERSION, of15::OFPT_GROUP_MOD, xid) {
    this->command_ = command;
    this->group_type_ = type;
    this->group_id_ = group_id;
    this->buckets_ = buckets;
    this->length_ = sizeof(struct of15::ofp_group_mod) + buckets_len();
}

bool GroupMod::operator==(const GroupMod &other) const {
    return ((OFMsg::operator==(other)) && (this->command_ == other.command_)
        && (this->group_type_ == other.group_type_)
        && (this->group_id_ == other.group_id_)
        && (this->buckets_ == other.buckets_));
}

bool GroupMod::operator!=(const GroupMod &other) const {
    return !(*this == other);
}

void GroupMod::buckets(std::vector<Bucket> buckets) {
    this->buckets_ = buckets;
    this->length_ += buckets_len();
}

void GroupMod::add_bucket(Bucket bucket) {
    this->buckets_.push_back(bucket);
    this->length_ += bucket.len();
}

size_t GroupMod::buckets_len() {
    size_t len = 0;
    for (std::vector<Bucket>::iterator it = this->buckets_.begin();
        it != this->buckets_.end(); ++it) {
        len += it->len();
    }
    return len;
}
;

uint8_t* GroupMod::pack() {
    uint8_t* buffer = OFMsg::pack();
    struct of15::ofp_group_mod *gm = (struct of15::ofp_group_mod*) buffer;
    gm->command = hton16(this->command_);
    gm->type = this->group_type_;
    gm->group_id = hton32(this->group_id_);
    uint8_t *p = buffer + sizeof(struct ofp_group_mod);
    for (std::vector<Bucket>::iterator it = this->buckets_.begin();
        it != this->buckets_.end(); ++it) {
        it->pack(p);
        p += it->len();
    }
    return buffer;
}

of_error GroupMod::unpack(uint8_t *buffer) {
    struct of15::ofp_group_mod *gm = (struct of15::ofp_group_mod*) buffer;
    OFMsg::unpack(buffer);
    if (this->length_ < sizeof(struct of15::ofp_group_mod)) {
        return openflow_error(of15::OFPET_BAD_REQUEST, of15::OFPBRC_BAD_LEN);
    }
    this->command_ = ntoh16(gm->command);
    this->group_type_ = gm->type;
    this->group_id_ = ntoh32(gm->group_id);
    size_t len = this->length_ - sizeof(struct ofp_group_mod);
    uint8_t *p = buffer + sizeof(struct ofp_group_mod);
    while (len) {
        Bucket bucket;
        bucket.unpack(p);
        this->buckets_.push_back(bucket);
        p += bucket.len();
        len -= bucket.len();
    }
    return 0;
}

TableMod::TableMod()
    : OFMsg(of15::OFP_VERSION, of15::OFPT_TABLE_MOD) {
    this->length_ = sizeof(struct of15::ofp_table_mod);
}

TableMod::TableMod(uint32_t xid, uint8_t table_id, uint32_t config)
    : OFMsg(of15::OFP_VERSION, of15::OFPT_TABLE_MOD, xid),
      table_id_(table_id),
      config_(config) {
    this->length_ = sizeof(struct of15::ofp_table_mod);
}

bool TableMod::operator==(const TableMod &other) const {
    return ((OFMsg::operator==(other)) && (this->table_id_ == other.table_id_)
        && (this->config_ == other.config_));
}

bool TableMod::operator!=(const TableMod &other) const {
    return !(*this == other);
}

uint8_t* TableMod::pack() {
    uint8_t* buffer = OFMsg::pack();
    struct of15::ofp_table_mod *tm = (struct of15::ofp_table_mod*) buffer;
    tm->table_id = this->table_id_;
    memset(tm->pad, 0x0, 3);
    tm->config = hton32(this->config_);
    return buffer;
}

of_error TableMod::unpack(uint8_t *buffer) {
    struct of15::ofp_table_mod *tm = (struct of15::ofp_table_mod*) buffer;
    OFMsg::unpack(buffer);
    if (this->length_ < sizeof(struct of15::ofp_table_mod)) {
        return openflow_error(of15::OFPET_BAD_REQUEST, of15::OFPBRC_BAD_LEN);
    }
    this->table_id_ = tm->table_id;
    this->config_ = ntoh32(tm->config);
    return 0;
}

MultipartRequest::MultipartRequest()
    : OFMsg(of15::OFP_VERSION, of15::OFPT_MULTIPART_REQUEST) {
    this->length_ = sizeof(struct of15::ofp_multipart_request);
}

MultipartRequest::MultipartRequest(uint16_t type)
    : OFMsg(of15::OFP_VERSION, of15::OFPT_MULTIPART_REQUEST),
      mpart_type_(type) {
    this->length_ = sizeof(struct of15::ofp_multipart_request);
}

MultipartRequest::MultipartRequest(uint32_t xid, uint16_t type, uint16_t flags)
    : OFMsg(of15::OFP_VERSION, of15::OFPT_MULTIPART_REQUEST, xid),
      mpart_type_(type),
      flags_(flags) {
    this->length_ = sizeof(struct of15::ofp_multipart_request);
}

bool MultipartRequest::operator==(const MultipartRequest &other) const {
    return ((OFMsg::operator==(other))
        && (this->mpart_type_ == other.mpart_type_)
        && (this->length_ == other.length_));
}

bool MultipartRequest::operator!=(const MultipartRequest &other) const {
    return !(*this == other);
}

uint8_t* MultipartRequest::pack() {
    uint8_t* buffer = OFMsg::pack();
    struct of15::ofp_multipart_request * mr =
        (struct of15::ofp_multipart_request *) buffer;
    mr->type = hton16(this->mpart_type_);
    mr->flags = hton16(this->flags_);
    memset(mr->pad, 0x0, 4);
    return buffer;
}

of_error MultipartRequest::unpack(uint8_t *buffer) {
    struct of15::ofp_multipart_request * mr =
        (struct of15::ofp_multipart_request *) buffer;
    OFMsg::unpack(buffer);
    if (this->length_ < sizeof(struct of15::ofp_multipart_request)) {
        return openflow_error(of15::OFPET_BAD_REQUEST, of15::OFPBRC_BAD_LEN);
    }
    this->mpart_type_ = ntoh16(mr->type);
    this->flags_ = ntoh16(mr->flags);
    return 0;
}

MultipartReply::MultipartReply()
    : OFMsg(of15::OFP_VERSION, of15::OFPT_MULTIPART_REPLY) {
    this->length_ = sizeof(struct of15::ofp_multipart_reply);
}

MultipartReply::MultipartReply(uint16_t type)
    : OFMsg(of15::OFP_VERSION, of15::OFPT_MULTIPART_REPLY),
      mpart_type_(type) {
    this->length_ = sizeof(struct of15::ofp_multipart_reply);
}

MultipartReply::MultipartReply(uint32_t xid, uint16_t type, uint16_t flags)
    : OFMsg(of15::OFP_VERSION, of15::OFPT_MULTIPART_REPLY, xid),
      mpart_type_(type),
      flags_(flags) {
    this->mpart_type_ = type;
    this->flags_ = flags;
    this->length_ = sizeof(struct of15::ofp_multipart_reply);
}

bool MultipartReply::operator==(const MultipartReply &other) const {
    return ((OFMsg::operator==(other))
        && (this->mpart_type_ == other.mpart_type_)
        && (this->flags_ == other.flags_));
}

bool MultipartReply::operator!=(const MultipartReply &other) const {
    return !(*this == other);
}

uint8_t* MultipartReply::pack() {
    uint8_t* buffer = OFMsg::pack();
    struct of15::ofp_multipart_reply * mr =
        (struct of15::ofp_multipart_reply *) buffer;
    mr->type = hton16(this->mpart_type_);
    mr->flags = hton16(this->flags_);
    memset(mr->pad, 0x0, 4);
    return buffer;
}

of_error MultipartReply::unpack(uint8_t *buffer) {
    struct of15::ofp_multipart_reply * mr =
        (struct of15::ofp_multipart_reply *) buffer;
    OFMsg::unpack(buffer);
    this->mpart_type_ = ntoh16(mr->type);
    this->flags_ = ntoh16(mr->flags);
    return 0;
}

MultipartRequestDesc::MultipartRequestDesc()
    : MultipartRequest(OFPMP_DESC) {
}

MultipartRequestDesc::MultipartRequestDesc(uint32_t xid, uint16_t flags)
    : MultipartRequest(xid, of15::OFPMP_DESC, flags) {
}

uint8_t* MultipartRequestDesc::pack() {
    uint8_t* buffer = MultipartRequest::pack();
    return buffer;
}

of_error MultipartRequestDesc::unpack(uint8_t *buffer) {
    return MultipartRequest::unpack(buffer);
}

MultipartReplyDesc::MultipartReplyDesc()
    : MultipartReply(of15::OFPMP_DESC) {
    this->length_ += sizeof(struct ofp_desc);
}

MultipartReplyDesc::MultipartReplyDesc(uint32_t xid, uint16_t flags,
    SwitchDesc desc)
    : MultipartReply(xid, of15::OFPMP_DESC, flags) {
    this->desc_ = desc;
    this->length_ += sizeof(struct ofp_desc);
}

MultipartReplyDesc::MultipartReplyDesc(uint32_t xid, uint16_t flags,
    std::string mfr_desc, std::string hw_desc, std::string sw_desc,
    std::string serial_num, std::string dp_desc)
    : MultipartReply(xid, of15::OFPMP_DESC, flags),
      desc_(mfr_desc, hw_desc, sw_desc, serial_num, dp_desc) {

    this->length_ += sizeof(struct ofp_desc);
}

bool MultipartReplyDesc::operator==(const MultipartReplyDesc &other) const {
    return ((MultipartReply::operator==(other)) && (this->desc_ == other.desc_));
}

bool MultipartReplyDesc::operator!=(const MultipartReplyDesc &other) const {
    return !(*this == other);
}

uint8_t* MultipartReplyDesc::pack() {
    uint8_t* buffer = MultipartReply::pack();
    this->desc_.pack(buffer + sizeof(struct of15::ofp_multipart_reply));
    return buffer;
}

of_error MultipartReplyDesc::unpack(uint8_t *buffer) {
    MultipartReply::unpack(buffer);
    this->desc_.unpack(buffer + sizeof(struct of15::ofp_multipart_reply));
    return 0;
}

MultipartRequestFlow::MultipartRequestFlow()
    : MultipartRequest(of15::OFPMP_FLOW_DESC) { //Aman
    this->length_ += sizeof(struct of15::ofp_flow_stats_request)
        - sizeof(struct of15::ofp_match);
}

MultipartRequestFlow::MultipartRequestFlow(uint32_t xid, uint16_t flags,
    uint8_t table_id, uint32_t out_port, uint32_t out_group, uint64_t cookie,
    uint64_t cookie_mask)
    : MultipartRequest(xid, of15::OFPMP_FLOW_DESC, flags), //Aman
      table_id_(table_id),
      out_port_(out_port),
      out_group_(out_group),
      cookie_(cookie),
      cookie_mask_(cookie_mask) {
    this->length_ += sizeof(struct of15::ofp_flow_stats_request)
        - sizeof(struct of15::ofp_match);
}

MultipartRequestFlow::MultipartRequestFlow(uint32_t xid, uint16_t flags,
    uint8_t table_id, uint32_t out_port, uint32_t out_group, uint64_t cookie,
    uint64_t cookie_mask, of15::Match match)
    : MultipartRequest(xid, of15::OFPMP_FLOW_DESC, flags), //Aman
      table_id_(table_id),
      out_port_(out_port),
      out_group_(out_group),
      cookie_(cookie),
      cookie_mask_(cookie_mask),
      match_(match) {

    this->length_ += sizeof(struct of15::ofp_flow_stats_request)
        - sizeof(struct of15::ofp_match) + match.length();
}

bool MultipartRequestFlow::operator==(const MultipartRequestFlow &other) const {
    return ((MultipartRequest::operator==(other))
        && (this->table_id_ == other.table_id_)
        && (this->out_port_ == other.out_port_)
        && (this->out_group_ == other.out_group_)
        && (this->cookie_ == other.cookie_)
        && (this->cookie_mask_ == other.cookie_mask_)
        && (this->match_ == other.match_));
}

bool MultipartRequestFlow::operator!=(const MultipartRequestFlow &other) const {
    return !(*this == other);
}

void MultipartRequestFlow::match(of15::Match match) {
    this->match_ = match;
    this->length_ += match.length();
}

void MultipartRequestFlow::add_oxm_field(OXMTLV &field) {
    this->match_.add_oxm_field(field);
}

void MultipartRequestFlow::add_oxm_field(OXMTLV* field) {
    this->match_.add_oxm_field(field);
}

uint8_t* MultipartRequestFlow::pack() {
    size_t padding = ROUND_UP(
        sizeof(struct of15::ofp_multipart_request)
            + sizeof(struct of15::ofp_flow_stats_request)
            - sizeof(struct of15::ofp_match) + this->match_.length(), 8)
        - (sizeof(struct of15::ofp_multipart_request)
            + sizeof(struct of15::ofp_flow_stats_request)
            - sizeof(struct of15::ofp_match) + this->match_.length());
    this->length_ = sizeof(struct of15::ofp_multipart_request)
        + sizeof(struct of15::ofp_flow_stats_request)
        - sizeof(struct of15::ofp_match) + ROUND_UP(this->match_.length(), 8);
    uint8_t* buffer = MultipartRequest::pack();
    struct of15::ofp_flow_stats_request *fs =
        (struct of15::ofp_flow_stats_request*) (buffer
            + sizeof(struct of15::ofp_multipart_request));
    fs->table_id = this->table_id_;
    memset(fs->pad, 0x0, 3);
    fs->out_port = hton32(this->out_port_);
    fs->out_group = hton32(this->out_group_);
    memset(fs->pad2, 0x0, 4);
    fs->cookie = hton64(this->cookie_);
    fs->cookie_mask = hton64(this->cookie_mask_);
    uint8_t *p = buffer
        + (sizeof(struct of15::ofp_multipart_request)
            + sizeof(struct of15::ofp_flow_stats_request)
            - sizeof(struct of15::ofp_match));
    this->match_.pack(p);
    p += this->match_.length();
    memset(p, 0x0, padding);
    return buffer;
}

of_error MultipartRequestFlow::unpack(uint8_t *buffer) {
    struct of15::ofp_flow_stats_request *fs =
        (struct of15::ofp_flow_stats_request*) (buffer
            + sizeof(struct of15::ofp_multipart_request));
    MultipartRequest::unpack(buffer);
    if (this->length_
        < sizeof(struct of15::ofp_multipart_request)
            + sizeof(struct of15::ofp_flow_stats_request)) {
        return openflow_error(of15::OFPET_BAD_REQUEST, of15::OFPBRC_BAD_LEN);
    }
    this->table_id_ = fs->table_id;
    this->out_port_ = ntoh32(fs->out_port);
    this->out_group_ = ntoh32(fs->out_group);
    this->cookie_ = ntoh64(fs->cookie);
    this->cookie_mask_ = ntoh64(fs->cookie_mask);
    uint8_t *p = buffer
        + (sizeof(struct of15::ofp_multipart_request)
            + sizeof(struct of15::ofp_flow_stats_request)
            - sizeof(struct of15::ofp_match));
    this->match_.unpack(p);
    return 0;
}

//TODO: Add support for other flow request types
MultipartReplyFlow::MultipartReplyFlow()
    : MultipartReply(of15::OFPMP_FLOW_DESC) { //Aman
}

MultipartReplyFlow::MultipartReplyFlow(uint32_t xid, uint16_t flags)
    : MultipartReply(xid, of15::OFPMP_FLOW_DESC, flags) { //Aman
}

MultipartReplyFlow::MultipartReplyFlow(uint32_t xid, uint16_t flags,
    std::vector<of15::FlowStats> flow_stats)
    : MultipartReply(xid, of15::OFPMP_FLOW_DESC, flags), //Aman
      flow_stats_(flow_stats) {
    size_t stats_len = 0;
    for (std::vector<of15::FlowStats>::iterator it = this->flow_stats_.begin();
        it != this->flow_stats_.end(); ++it) {
        stats_len += it->length();
    }
    this->length_ += stats_len;
}

bool MultipartReplyFlow::operator==(const MultipartReplyFlow &other) const {
    return ((MultipartReply::operator==(other))
        && (this->flow_stats_ == other.flow_stats_));
}

bool MultipartReplyFlow::operator!=(const MultipartReplyFlow &other) const {
    return !(*this == other);
}

uint8_t* MultipartReplyFlow::pack() {
    uint8_t* buffer = MultipartReply::pack();
    uint8_t *p = buffer + sizeof(struct of15::ofp_multipart_reply);
    for (std::vector<of15::FlowStats>::iterator it = this->flow_stats_.begin();
        it != this->flow_stats_.end(); ++it) {
        it->pack(p);
        p += it->length();
    }
    return buffer;
}

of_error MultipartReplyFlow::unpack(uint8_t *buffer) {
    MultipartReply::unpack(buffer);
    size_t len = this->length_ - sizeof(struct of15::ofp_multipart_reply);
    uint8_t *p = buffer + sizeof(struct of15::ofp_multipart_reply);
    while (len) {
        of15::FlowStats stat;
        stat.unpack(p);
        this->flow_stats_.push_back(stat);
        p += stat.length();
        len -= stat.length();
    }
    return 0;
}

void MultipartReplyFlow::flow_stats(std::vector<of15::FlowStats> flow_stats) {
    this->flow_stats_ = flow_stats;
    this->length_ += this->flow_stats_.size()
        * sizeof(struct of15::ofp_flow_stats);
}

void MultipartReplyFlow::add_flow_stats(of15::FlowStats stats) {
    this->flow_stats_.push_back(stats);
    this->length_ += stats.length();
}

MultipartRequestAggregate::MultipartRequestAggregate()
    : MultipartRequest(of15::OFPMP_AGGREGATE_STATS) { //Aman
    this->length_ += sizeof(struct of15::ofp_aggregate_stats_request)
        - sizeof(struct of15::ofp_match);
}

MultipartRequestAggregate::MultipartRequestAggregate(uint32_t xid,
    uint16_t flags, uint8_t table_id, uint32_t out_port, uint32_t out_group,
    uint64_t cookie, uint64_t cookie_mask)
    : MultipartRequest(xid, of15::OFPMP_AGGREGATE_STATS, flags), //Aman
      table_id_(table_id),
      out_port_(out_port),
      out_group_(out_group),
      cookie_(cookie),
      cookie_mask_(cookie_mask) {
    this->length_ += sizeof(struct of15::ofp_aggregate_stats_request)
        - sizeof(struct of15::ofp_match);
}

MultipartRequestAggregate::MultipartRequestAggregate(uint32_t xid,
    uint16_t flags, uint8_t table_id, uint32_t out_port, uint32_t out_group,
    uint64_t cookie, uint64_t cookie_mask, of15::Match match)
    : MultipartRequest(xid, of15::OFPMP_AGGREGATE_STATS, flags), //Aman
      table_id_(table_id),
      out_port_(out_port),
      out_group_(out_group),
      cookie_(cookie),
      cookie_mask_(cookie_mask),
      match_(match) {
    this->length_ = length();
}

bool MultipartRequestAggregate::operator==(
    const MultipartRequestAggregate &other) const {
    return ((MultipartRequest::operator==(other))
        && (this->table_id_ == other.table_id_)
        && (this->out_port_ == other.out_port_)
        && (this->out_group_ == other.out_group_)
        && (this->cookie_ == other.cookie_)
        && (this->cookie_mask_ == other.cookie_mask_)
        && (this->match_ == other.match_));
}

bool MultipartRequestAggregate::operator!=(
    const MultipartRequestAggregate &other) const {
    return !(*this == other);
}

uint16_t MultipartRequestAggregate::length() {
    return sizeof(struct of15::ofp_multipart_request)
        + sizeof(struct of15::ofp_aggregate_stats_request)
        - sizeof(struct of15::ofp_match) + ROUND_UP(this->match_.length(), 8);
}

void MultipartRequestAggregate::match(of15::Match match) {
    this->match_ = match;
    this->length_ += ROUND_UP(match_.length(), 8);
}

void MultipartRequestAggregate::add_oxm_field(OXMTLV &field) {
    this->match_.add_oxm_field(field);
}

void MultipartRequestAggregate::add_oxm_field(OXMTLV* field) {
    this->match_.add_oxm_field(field);
}

uint8_t* MultipartRequestAggregate::pack() {
    size_t padding = length()
        - (sizeof(struct of15::ofp_multipart_request)
            + sizeof(struct of15::ofp_aggregate_stats_request)
            - sizeof(struct of15::ofp_match) + this->match_.length());
    this->length_ = length();
    uint8_t* buffer = MultipartRequest::pack();
    struct of15::ofp_aggregate_stats_request *fs =
        (struct of15::ofp_aggregate_stats_request*) (buffer
            + sizeof(struct of15::ofp_multipart_request));
    fs->table_id = this->table_id_;
    memset(fs->pad, 0x0, 3);
    fs->out_port = hton32(this->out_port_);
    fs->out_group = hton32(this->out_group_);
    memset(fs->pad2, 0x0, 4);
    fs->cookie = hton64(this->cookie_);
    fs->cookie_mask = hton64(this->cookie_mask_);
    uint8_t *p = buffer
        + (sizeof(struct of15::ofp_multipart_request)
            + sizeof(struct of15::ofp_aggregate_stats_request)
            - sizeof(struct of15::ofp_match));
    this->match_.pack(p);
    p += this->match_.length();
    memset(p, 0x0, padding);
    return buffer;
}

of_error MultipartRequestAggregate::unpack(uint8_t *buffer) {
    struct of15::ofp_aggregate_stats_request *fs =
        (struct of15::ofp_aggregate_stats_request*) (buffer
            + sizeof(struct of15::ofp_multipart_request));
    MultipartRequest::unpack(buffer);
    if (this->length_
        < sizeof(struct of15::ofp_multipart_request)
            + sizeof(struct of15::ofp_aggregate_stats_request)) {
        return openflow_error(of15::OFPET_BAD_REQUEST, of15::OFPBRC_BAD_LEN);
    }
    this->table_id_ = fs->table_id;
    this->out_port_ = ntoh32(fs->out_port);
    this->out_group_ = ntoh32(fs->out_group);
    this->cookie_ = ntoh64(fs->cookie);
    this->cookie_mask_ = ntoh64(fs->cookie_mask);
    uint8_t *p = buffer
        + (sizeof(struct of15::ofp_multipart_request)
            + sizeof(struct of15::ofp_aggregate_stats_request)
            - sizeof(struct of15::ofp_match));
    this->match_.unpack(p);
    return 0;
}

MultipartReplyAggregate::MultipartReplyAggregate()
    : MultipartReply(of15::OFPMP_AGGREGATE_STATS) { //Aman
    this->length_ += sizeof(struct of15::ofp_aggregate_stats_reply);
}

MultipartReplyAggregate::MultipartReplyAggregate(uint32_t xid, uint16_t flags,
    uint64_t packet_count, uint64_t byte_count, uint32_t flow_count)
    : MultipartReply(xid, of15::OFPMP_AGGREGATE_STATS, flags), //Aman
      packet_count_(packet_count),
      byte_count_(byte_count),
      flow_count_(flow_count) {
    this->length_ += sizeof(struct of15::ofp_aggregate_stats_reply);
}

bool MultipartReplyAggregate::operator==(
    const MultipartReplyAggregate &other) const {
    return ((MultipartReply::operator==(other))
        && (this->packet_count_ == other.packet_count_)
        && (this->byte_count_ == other.byte_count_)
        && (this->flow_count_ == other.flow_count_));
}

bool MultipartReplyAggregate::operator!=(
    const MultipartReplyAggregate &other) const {
    return !(*this == other);
}

uint8_t* MultipartReplyAggregate::pack() {
    uint8_t* buffer = MultipartReply::pack();
    struct of15::ofp_aggregate_stats_reply *ar =
        (struct of15::ofp_aggregate_stats_reply*) (buffer
            + sizeof(struct of15::ofp_multipart_reply));
//Aman
//    ar->packet_count = hton64(this->packet_count_);
//    ar->byte_count = hton64(this->byte_count_);
//    ar->flow_count = hton32(this->flow_count_);
    return buffer;
}

of_error MultipartReplyAggregate::unpack(uint8_t *buffer) {
    struct of15::ofp_aggregate_stats_reply *ar =
        (struct of15::ofp_aggregate_stats_reply*) (buffer
            + sizeof(struct of15::ofp_multipart_reply));
    MultipartReply::unpack(buffer);
//Aman
//    this->packet_count_ = ntoh64(ar->packet_count);
//    this->byte_count_ = ntoh64(ar->byte_count);
//    this->flow_count_ = ntoh32(ar->flow_count);
    return 0;
}

//TODO: Add other table request/reply type support
MultipartRequestTable::MultipartRequestTable()
    : MultipartRequest(of15::OFPMP_TABLE_DESC) { //Aman
}

MultipartRequestTable::MultipartRequestTable(uint32_t xid, uint16_t flags)
    : MultipartRequest(xid, of15::OFPMP_TABLE_DESC, flags) { //Aman
}

uint8_t* MultipartRequestTable::pack() {
    uint8_t* buffer = MultipartRequest::pack();
    return buffer;
}

of_error MultipartRequestTable::unpack(uint8_t *buffer) {
    return MultipartRequest::unpack(buffer);
}

MultipartReplyTable::MultipartReplyTable()
    : MultipartReply(of15::OFPMP_TABLE_DESC) { //Aman
}

MultipartReplyTable::MultipartReplyTable(uint32_t xid, uint16_t flags)
    : MultipartReply(xid, of15::OFPMP_TABLE_DESC, flags) { //Aman
}

MultipartReplyTable::MultipartReplyTable(uint32_t xid, uint16_t flags,
    std::vector<of15::TableStats> table_stats)
    : MultipartReply(xid, of15::OFPMP_TABLE_DESC, flags), //Aman
      table_stats_(table_stats) {
    this->length_ += table_stats.size() * sizeof(struct of15::ofp_table_stats);

}

bool MultipartReplyTable::operator==(const MultipartReplyTable &other) const {
    return ((MultipartReply::operator==(other))
        && (this->table_stats_ == other.table_stats_));
}

bool MultipartReplyTable::operator!=(const MultipartReplyTable &other) const {
    return !(*this == other);
}

uint8_t* MultipartReplyTable::pack() {
    uint8_t* buffer = MultipartReply::pack();
    uint8_t *p = buffer + sizeof(struct ofp_multipart_request);
    for (std::vector<of15::TableStats>::iterator it =
        this->table_stats_.begin(); it != this->table_stats_.end(); ++it) {
        it->pack(p);
        p += sizeof(struct of15::ofp_table_stats);
    }
    return buffer;
}

of_error MultipartReplyTable::unpack(uint8_t *buffer) {
    MultipartReply::unpack(buffer);
    uint8_t len = this->length_ - sizeof(struct of15::ofp_multipart_request);
    uint8_t *p = buffer + sizeof(struct of15::ofp_multipart_request);
    while (len) {
        TableStats stat;
        stat.unpack(p);
        this->table_stats_.push_back(stat);
        p += sizeof(struct of15::ofp_table_stats);
        len -= sizeof(struct of15::ofp_table_stats);
    }
    return 0;
}

void MultipartReplyTable::table_stats(
    std::vector<of15::TableStats> table_stats) {
    this->table_stats_ = table_stats;
    this->length_ += table_stats.size() * sizeof(struct of15::ofp_table_stats);
}

void MultipartReplyTable::add_table_stat(of15::TableStats stat) {
    this->table_stats_.push_back(stat);
    this->length_ += sizeof(struct of15::ofp_table_stats);
}

MultipartRequestPortStats::MultipartRequestPortStats()
    : MultipartRequest(of15::OFPMP_PORT_STATS) { //Aman
    this->length_ += sizeof(struct of15::ofp_port_stats_prop_header); //Aman
}

MultipartRequestPortStats::MultipartRequestPortStats(uint32_t xid,
    uint16_t flags, uint32_t port_no)
    : MultipartRequest(xid, of15::OFPMP_PORT_STATS, flags),
      port_no_(port_no) {
    this->length_ += sizeof(struct of15::ofp_port_stats_prop_header); //Aman

}
;

bool MultipartRequestPortStats::operator==(
    const MultipartRequestPortStats &other) const {
    return ((MultipartRequest::operator==(other))
        && (this->port_no_ == other.port_no_));
}

bool MultipartRequestPortStats::operator!=(
    const MultipartRequestPortStats &other) const {
    return !(*this == other);
}

uint8_t* MultipartRequestPortStats::pack() {
    uint8_t* buffer = MultipartRequest::pack();
    struct of15::ofp_port_stats *ps = //Aman
        (struct of15::ofp_port_stats *) (buffer //Aman
            + sizeof(struct of15::ofp_multipart_request));
    ps->port_no = hton32(this->port_no_);
    memset(ps->pad, 0x0, 4);
    return buffer;
}

of_error MultipartRequestPortStats::unpack(uint8_t *buffer) {
    struct of15::ofp_port_stats *ps = //Aman
        (struct of15::ofp_port_stats *) (buffer //Aman
            + sizeof(struct of15::ofp_multipart_request));
    MultipartRequest::unpack(buffer);
    if (this->length_
        < sizeof(struct of15::ofp_multipart_request)
            + sizeof(struct of15::ofp_port_stats)) { //Aman
        return openflow_error(of15::OFPET_BAD_REQUEST, of15::OFPBRC_BAD_LEN);
    }
    this->port_no_ = ntoh32(ps->port_no);
    return 0;
}

MultipartReplyPortStats::MultipartReplyPortStats()
    : MultipartReply(OFPMP_PORT_STATS) {
}

MultipartReplyPortStats::MultipartReplyPortStats(uint32_t xid, uint16_t flags)
    : MultipartReply(xid, of15::OFPMP_PORT_STATS, flags) {
}

MultipartReplyPortStats::MultipartReplyPortStats(uint32_t xid, uint16_t flags,
    std::vector<of15::PortStats> port_stats)
    : MultipartReply(xid, of15::OFPMP_PORT_STATS, flags) {
    this->port_stats_ = port_stats;
    this->length_ = port_stats.size() * sizeof(struct of15::ofp_port_stats);

}

bool MultipartReplyPortStats::operator==(
    const MultipartReplyPortStats &other) const {
    return ((MultipartReply::operator==(other))
        && (this->port_stats_ == other.port_stats_));
}

bool MultipartReplyPortStats::operator!=(
    const MultipartReplyPortStats &other) const {
    return !(*this == other);
}

uint8_t* MultipartReplyPortStats::pack() {
    uint8_t* buffer = MultipartReply::pack();
    uint8_t *p = buffer + sizeof(struct ofp_multipart_request);
    for (std::vector<of15::PortStats>::iterator it = this->port_stats_.begin();
        it != this->port_stats_.end(); ++it) {
        it->pack(p);
        p += sizeof(struct of15::ofp_port_stats);
    }
    return buffer;
}

of_error MultipartReplyPortStats::unpack(uint8_t *buffer) {
    MultipartReply::unpack(buffer);
    uint8_t len = this->length_ - sizeof(struct of15::ofp_multipart_request);
    uint8_t *p = buffer + sizeof(struct of15::ofp_multipart_request);
    while (len) {
        of15::PortStats stat;
        stat.unpack(p);
        this->port_stats_.push_back(stat);
        p += sizeof(struct of15::ofp_port_stats);
        len -= sizeof(struct of15::ofp_port_stats);
    }
    return 0;
}

void MultipartReplyPortStats::port_stats(
    std::vector<of15::PortStats> port_stats) {
    this->port_stats_ = port_stats;
    this->length_ += port_stats.size() * sizeof(struct of15::ofp_port_stats);
}

void MultipartReplyPortStats::add_port_stat(of15::PortStats stat) {
    this->port_stats_.push_back(stat);
    this->length_ += sizeof(struct of15::ofp_port_stats);
}

//TODO handle other OFPMP_QUEUE_ requests
MultipartRequestQueue::MultipartRequestQueue()
    : MultipartRequest(of15::OFPMP_QUEUE_STATS) { //Aman
    this->length_ += sizeof(struct of15::ofp_queue_stats); //Aman
}

MultipartRequestQueue::MultipartRequestQueue(uint32_t xid, uint16_t flags,
    uint32_t port_no, uint32_t queue_id)
    : MultipartRequest(xid, of15::OFPMP_QUEUE_STATS, flags), //Aman
      port_no_(port_no),
      queue_id_(queue_id) {
    this->length_ += sizeof(struct of15::ofp_queue_stats); //Aman
}

bool MultipartRequestQueue::operator==(
    const MultipartRequestQueue &other) const {
    return ((MultipartRequest::operator==(other))
        && (this->queue_id_ == other.queue_id_)
        && (this->port_no_ == other.port_no_));
}

bool MultipartRequestQueue::operator!=(
    const MultipartRequestQueue &other) const {
    return !(*this == other);
}

uint8_t* MultipartRequestQueue::pack() {
    uint8_t* buffer = MultipartRequest::pack();
    struct of15::ofp_queue_stats* qs = //Aman
        (of15::ofp_queue_stats*) (buffer //Aman
            + sizeof(struct of15::ofp_multipart_request));
    qs->port_no = hton32(this->port_no_);
    qs->queue_id = hton32(this->queue_id_);
    return buffer;
}

of_error MultipartRequestQueue::unpack(uint8_t *buffer) {
    struct of15::ofp_queue_stats* qs = //Aman
        (of15::ofp_queue_stats*) (buffer -//Aman
            + sizeof(struct of15::ofp_multipart_request));
    MultipartRequest::unpack(buffer);
    if (this->length_
        < sizeof(struct of15::ofp_multipart_request)
            + sizeof(struct of15::ofp_queue_stats)) { //Aman
        return openflow_error(of15::OFPET_BAD_REQUEST, of15::OFPBRC_BAD_LEN);
    }
    this->port_no_ = ntoh32(qs->port_no);
    this->queue_id_ = ntoh32(qs->queue_id);
    return 0;
}

//TODO: Handle other OFPMP_QUEUE_ reply types
MultipartReplyQueue::MultipartReplyQueue()
    : MultipartReply(of15::OFPMP_QUEUE_STATS) { //Aman
}

MultipartReplyQueue::MultipartReplyQueue(uint32_t xid, uint16_t flags)
    : MultipartReply(xid, of15::OFPMP_QUEUE_STATS, flags) { //Aman
}

MultipartReplyQueue::MultipartReplyQueue(uint32_t xid, uint16_t flags,
    std::vector<of15::QueueStats> queue_stats)
    : MultipartReply(xid, of15::OFPMP_QUEUE_STATS, flags), //Aman
      queue_stats_(queue_stats) { //Aman
    this->length_ += queue_stats.size() * sizeof(struct of15::ofp_queue_stats);
}

bool MultipartReplyQueue::operator==(const MultipartReplyQueue &other) const {
    return ((MultipartReply::operator==(other))
        && (this->queue_stats_ == other.queue_stats_));
}

bool MultipartReplyQueue::operator!=(const MultipartReplyQueue &other) const {
    return !(*this == other);
}

uint8_t* MultipartReplyQueue::pack() {
    uint8_t* buffer = MultipartReply::pack();
    uint8_t *p = buffer + sizeof(struct of15::ofp_multipart_reply);
    for (std::vector<of15::QueueStats>::iterator it =
        this->queue_stats_.begin(); it != this->queue_stats_.end(); ++it) {
        it->pack(p);
        p += sizeof(struct of15::ofp_queue_stats);
    }
    return buffer;
}

of_error MultipartReplyQueue::unpack(uint8_t *buffer) {
    MultipartReply::unpack(buffer);
    int len = this->length_ - sizeof(struct of15::ofp_multipart_reply);
    uint8_t *p = buffer + sizeof(struct of15::ofp_multipart_reply);
    while (len > 0) {
        of15::QueueStats stat;
        stat.unpack(p);
        this->queue_stats_.push_back(stat);
        p += sizeof(struct of15::ofp_queue_stats);
        len -= sizeof(struct of15::ofp_queue_stats);
    }
    return 0;
}

void MultipartReplyQueue::queue_stats(
    std::vector<of15::QueueStats> queue_stats) {
    this->queue_stats_ = queue_stats;
    this->length_ += queue_stats.size() * sizeof(struct of15::ofp_queue_stats);
}

void MultipartReplyQueue::add_queue_stat(of15::QueueStats stat) {
    this->queue_stats_.push_back(stat);
    this->length_ += sizeof(struct of15::ofp_queue_stats); //Aman
}

//TODO: Handle other OFPMP_GROUP_ request types
MultipartRequestGroup::MultipartRequestGroup()
    : MultipartRequest(of15::OFPMP_GROUP_STATS) { //Aman
    this->length_ += sizeof(struct of15::ofp_group_stats); //Aman
}

MultipartRequestGroup::MultipartRequestGroup(uint32_t xid, uint16_t flags,
    uint32_t group_id)
    : MultipartRequest(xid, of15::OFPMP_GROUP_STATS, flags),
      group_id_(group_id) {
    this->length_ += sizeof(struct of15::ofp_group_stats); //Aman
}

bool MultipartRequestGroup::operator==(
    const MultipartRequestGroup &other) const {
    return ((MultipartRequest::operator==(other))
        && (this->group_id_ == other.group_id_));
}

bool MultipartRequestGroup::operator!=(
    const MultipartRequestGroup &other) const {
    return !(*this == other);
}

uint8_t* MultipartRequestGroup::pack() {
    uint8_t* buffer = MultipartRequest::pack();
    struct of15::ofp_group_stats* gs = //Aman
        (of15::ofp_group_stats*) (buffer //Aman
            + sizeof(struct of15::ofp_multipart_request));
    gs->group_id = hton32(this->group_id_);
    memset(gs->pad, 0x0, 4);
    return buffer;
}

of_error MultipartRequestGroup::unpack(uint8_t *buffer) {
    struct of15::ofp_group_stats* gs = //Aman
        (of15::ofp_group_stats*) (buffer //Aman
            + sizeof(struct of15::ofp_multipart_request));
    MultipartRequest::unpack(buffer);
    if (this->length_
        < sizeof(struct of15::ofp_multipart_request)
            + sizeof(struct of15::ofp_group_stats)) { //Aman
        return openflow_error(of15::OFPET_BAD_REQUEST, of15::OFPBRC_BAD_LEN);
    }
    this->group_id_ = ntoh32(gs->group_id);
    return 0;
}

//TODO: Handler other OFPMP_GROUP_ reply types
MultipartReplyGroup::MultipartReplyGroup()
    : MultipartReply(of15::OFPMP_GROUP_STATS) { //Aman
}

MultipartReplyGroup::MultipartReplyGroup(uint32_t xid, uint16_t flags)
    : MultipartReply(xid, of15::OFPMP_GROUP_STATS, flags) { //Aman
}

MultipartReplyGroup::MultipartReplyGroup(uint32_t xid, uint16_t flags,
    std::vector<of15::GroupStats> group_stats)
    : MultipartReply(xid, of15::OFPMP_GROUP_STATS, flags), //Aman
      group_stats_(group_stats) {
    this->length_ += group_stats_len();
}

bool MultipartReplyGroup::operator==(const MultipartReplyGroup &other) const {
    return ((MultipartReply::operator==(other))
        && (this->group_stats_ == other.group_stats_));
}

bool MultipartReplyGroup::operator!=(const MultipartReplyGroup &other) const {
    return !(*this == other);
}

uint8_t* MultipartReplyGroup::pack() {
    uint8_t* buffer = MultipartReply::pack();
    uint8_t *p = buffer + sizeof(struct of15::ofp_multipart_reply);
    for (std::vector<of15::GroupStats>::iterator it =
        this->group_stats_.begin(); it != this->group_stats_.end(); ++it) {
        it->pack(p);
        p += it->length();
    }
    return buffer;
}

of_error MultipartReplyGroup::unpack(uint8_t *buffer) {
    MultipartReply::unpack(buffer);
    uint8_t len = this->length_ - sizeof(struct of15::ofp_multipart_reply);
    uint8_t *p = buffer + sizeof(struct of15::ofp_multipart_reply);
    while (len) {
        of15::GroupStats stat;
        stat.unpack(p);
        this->group_stats_.push_back(stat);
        p += stat.length();
        len -= stat.length();
    }
    return 0;
}

void MultipartReplyGroup::group_stats(
    std::vector<of15::GroupStats> group_stats) {
    this->group_stats_ = group_stats;
    this->length_ += group_stats_len();
}

void MultipartReplyGroup::add_group_stats(of15::GroupStats stat) {
    this->group_stats_.push_back(stat);
    this->length_ += stat.length();
}

size_t MultipartReplyGroup::group_stats_len() {
    size_t len;
    for (std::vector<of15::GroupStats>::iterator it =
        this->group_stats_.begin(); it != this->group_stats_.end(); ++it) {
        len += it->length();
    }
    return len;
}

MultipartRequestGroupDesc::MultipartRequestGroupDesc()
    : MultipartRequest(OFPMP_GROUP_DESC) {
}

MultipartRequestGroupDesc::MultipartRequestGroupDesc(uint32_t xid,
    uint16_t flags)
    : MultipartRequest(xid, of15::OFPMP_GROUP_DESC, flags) {
}

uint8_t* MultipartRequestGroupDesc::pack() {
    uint8_t* buffer = MultipartRequest::pack();
    return buffer;
}

of_error MultipartRequestGroupDesc::unpack(uint8_t *buffer) {
    return MultipartRequest::unpack(buffer);
}

MultipartReplyGroupDesc::MultipartReplyGroupDesc()
    : MultipartReply(OFPMP_GROUP_DESC) {
}

MultipartReplyGroupDesc::MultipartReplyGroupDesc(uint32_t xid, uint16_t flags)
    : MultipartReply(xid, of15::OFPMP_GROUP_DESC, flags) {
}

MultipartReplyGroupDesc::MultipartReplyGroupDesc(uint32_t xid, uint16_t flags,
    std::vector<of15::GroupDesc> group_desc)
    : MultipartReply(xid, of15::OFPMP_GROUP_DESC, flags),
      group_desc_(group_desc) {
    this->length_ += desc_len();
}

bool MultipartReplyGroupDesc::operator==(
    const MultipartReplyGroupDesc &other) const {
    return ((MultipartReply::operator==(other))
        && (this->group_desc_ == other.group_desc_));
}

bool MultipartReplyGroupDesc::operator!=(
    const MultipartReplyGroupDesc &other) const {
    return !(*this == other);
}

uint8_t* MultipartReplyGroupDesc::pack() {
    uint8_t* buffer = MultipartReply::pack();
    uint8_t *p = buffer + sizeof(struct of15::ofp_multipart_reply);
    for (std::vector<of15::GroupDesc>::iterator it = this->group_desc_.begin();
        it != this->group_desc_.end(); ++it) {
        it->pack(p);
        p += it->length();
    }
    return buffer;
}

of_error MultipartReplyGroupDesc::unpack(uint8_t *buffer) {
    MultipartReply::unpack(buffer);
    uint8_t len = this->length_ - sizeof(struct of15::ofp_multipart_reply);
    uint8_t *p = buffer + sizeof(struct of15::ofp_multipart_reply);
    while (len) {
        of15::GroupDesc desc;
        desc.unpack(p);
        this->group_desc_.push_back(desc);
        p += desc.length();
        len -= desc.length();
    }
    return 0;
}

void MultipartReplyGroupDesc::group_desc(
    std::vector<of15::GroupDesc> group_desc) {
    this->group_desc_ = group_desc;
    this->length_ += desc_len();
}

void MultipartReplyGroupDesc::add_group_desc(of15::GroupDesc desc) {
    this->group_desc_.push_back(desc);
    this->length_ += desc.length();
}

size_t MultipartReplyGroupDesc::desc_len() {
    size_t len = 0;
    for (std::vector<GroupDesc>::iterator it = this->group_desc_.begin();
        it != this->group_desc_.end(); ++it) {
        len += it->length();
    }
    return len;
}

MultipartRequestGroupFeatures::MultipartRequestGroupFeatures()
    : MultipartRequest(OFPMP_GROUP_FEATURES) {
}

MultipartRequestGroupFeatures::MultipartRequestGroupFeatures(uint32_t xid,
    uint16_t flags)
    : MultipartRequest(xid, of15::OFPMP_GROUP_FEATURES, flags) {
}

uint8_t* MultipartRequestGroupFeatures::pack() {
    uint8_t* buffer = MultipartRequest::pack();
    return buffer;
}

of_error MultipartRequestGroupFeatures::unpack(uint8_t *buffer) {
    return MultipartRequest::unpack(buffer);
}

MultipartReplyGroupFeatures::MultipartReplyGroupFeatures()
    : MultipartReply(OFPMP_GROUP_FEATURES) {
}

MultipartReplyGroupFeatures::MultipartReplyGroupFeatures(uint32_t xid,
    uint16_t flags, of15::GroupFeatures features)
    : MultipartReply(xid, of15::OFPMP_GROUP_FEATURES, flags),
      features_(features) {
    this->features_ = features;
    this->length_ += sizeof(struct of15::ofp_group_features);
}

bool MultipartReplyGroupFeatures::operator==(
    const MultipartReplyGroupFeatures &other) const {
    return ((MultipartReply::operator==(other))
        && (this->features_ == other.features_));
}

bool MultipartReplyGroupFeatures::operator!=(
    const MultipartReplyGroupFeatures &other) const {
    return !(*this == other);
}

uint8_t* MultipartReplyGroupFeatures::pack() {
    uint8_t* buffer = MultipartReply::pack();
    this->features_.pack(buffer + sizeof(struct of15::ofp_multipart_reply));
    return buffer;
}

of_error MultipartReplyGroupFeatures::unpack(uint8_t *buffer) {
    MultipartReply::unpack(buffer);
    this->features_.unpack(buffer + sizeof(struct of15::ofp_multipart_reply));
    return 0;
}

//TODO: Handle other OFPMP_METER_ request types
MultipartRequestMeter::MultipartRequestMeter()
    : MultipartRequest(of15::OFPMP_METER_STATS) { //Aman
    this->length_ += sizeof(struct of15::ofp_meter_multipart_request);
}

MultipartRequestMeter::MultipartRequestMeter(uint32_t xid, uint16_t flags,
    uint32_t meter_id)
    : MultipartRequest(xid, of15::OFPMP_METER_STATS, flags), //Aman
      meter_id_(meter_id) {
    this->length_ += sizeof(struct of15::ofp_meter_multipart_request);
}

bool MultipartRequestMeter::operator==(
    const MultipartRequestMeter &other) const {
    return ((MultipartRequest::operator==(other))
        && (this->meter_id_ == other.meter_id_));
}

bool MultipartRequestMeter::operator!=(
    const MultipartRequestMeter &other) const {
    return !(*this == other);
}

uint8_t* MultipartRequestMeter::pack() {
    uint8_t* buffer = MultipartRequest::pack();
    struct of15::ofp_meter_multipart_request *mr =
        (struct of15::ofp_meter_multipart_request *) (buffer
            + sizeof(struct of15::ofp_multipart_reply));
    mr->meter_id = hton32(this->meter_id_);
    memset(mr->pad, 0x0, 4);
    return buffer;
}

of_error MultipartRequestMeter::unpack(uint8_t *buffer) {
    struct of15::ofp_meter_multipart_request *mr =
        (struct of15::ofp_meter_multipart_request *) (buffer
            + sizeof(struct of15::ofp_multipart_reply));
    MultipartRequest::unpack(buffer);
    if (this->length_
        < sizeof(struct of15::ofp_multipart_request)
            + sizeof(struct of15::ofp_meter_multipart_request)) {
        return openflow_error(of15::OFPET_BAD_REQUEST, of15::OFPBRC_BAD_LEN);
    }
    this->meter_id_ = ntoh32(mr->meter_id);
    return 0;
}

//TODO: handle other OFPMP_METER_ reply types
MultipartReplyMeter::MultipartReplyMeter()
    : MultipartReply(of15::OFPMP_METER_STATS) { //Aman
}

MultipartReplyMeter::MultipartReplyMeter(uint32_t xid, uint16_t flags)
    : MultipartReply(xid, of15::OFPMP_METER_STATS, flags) { //Aman
}

MultipartReplyMeter::MultipartReplyMeter(uint32_t xid, uint16_t flags,
    std::vector<of15::MeterStats> meter_stats)
    : MultipartReply(xid, of15::OFPMP_METER_STATS, flags), //Aman
      meter_stats_(meter_stats) {
    this->length_ += meter_stats_len();
}

bool MultipartReplyMeter::operator==(const MultipartReplyMeter &other) const {
    return ((MultipartReply::operator==(other))
        && (this->meter_stats_ == other.meter_stats_));
}

bool MultipartReplyMeter::operator!=(const MultipartReplyMeter &other) const {
    return !(*this == other);
}

uint8_t* MultipartReplyMeter::pack() {
    uint8_t* buffer = MultipartReply::pack();
    uint8_t *p = buffer + sizeof(struct of15::ofp_multipart_reply);
    for (std::vector<of15::MeterStats>::iterator it =
        this->meter_stats_.begin(); it != this->meter_stats_.end(); ++it) {
        it->pack(p);
        p += it->len();
    }
    return buffer;
}

of_error MultipartReplyMeter::unpack(uint8_t *buffer) {
    MultipartReply::unpack(buffer);
    uint8_t len = this->length_ - sizeof(struct of15::ofp_multipart_reply);
    uint8_t *p = buffer + sizeof(struct of15::ofp_multipart_reply);
    while (len) {
        of15::MeterStats stat;
        stat.unpack(p);
        this->meter_stats_.push_back(stat);
        p += stat.len();
        len -= stat.len();
    }
    return 0;
}

void MultipartReplyMeter::meter_stats(
    std::vector<of15::MeterStats> meter_stats) {
    this->meter_stats_ = meter_stats;
    this->length_ += meter_stats_len();
}

void MultipartReplyMeter::add_meter_stats(of15::MeterStats stat) {
    this->meter_stats_.push_back(stat);
    this->length_ += stat.len();
}

size_t MultipartReplyMeter::meter_stats_len() {
    size_t len;
    for (std::vector<of15::MeterStats>::iterator it =
        this->meter_stats_.begin(); it != this->meter_stats_.end(); ++it) {
        len += it->len();
    }
    return len;
}

MultipartRequestMeterConfig::MultipartRequestMeterConfig()
    : MultipartRequest(of15::OFPMP_METER_DESC) { //Aman
    this->length_ += sizeof(struct of15::ofp_meter_multipart_request);
}

MultipartRequestMeterConfig::MultipartRequestMeterConfig(uint32_t xid,
    uint16_t flags, uint32_t meter_id)
    : MultipartRequest(xid, of15::OFPMP_METER_DESC, flags), //Aman
      meter_id_(meter_id) {
    this->length_ += sizeof(struct of15::ofp_meter_multipart_request);
}

bool MultipartRequestMeterConfig::operator==(
    const MultipartRequestMeterConfig &other) const {
    return ((MultipartRequest::operator==(other))
        && (this->meter_id_ == other.meter_id_));
}

bool MultipartRequestMeterConfig::operator!=(
    const MultipartRequestMeterConfig &other) const {
    return !(*this == other);
}

uint8_t* MultipartRequestMeterConfig::pack() {
    uint8_t* buffer = MultipartRequest::pack();
    struct of15::ofp_meter_multipart_request *mr =
        (struct of15::ofp_meter_multipart_request *) (buffer
            + sizeof(struct of15::ofp_multipart_reply));
    mr->meter_id = hton32(this->meter_id_);
    memset(mr->pad, 0x0, 4);
    return buffer;
}

of_error MultipartRequestMeterConfig::unpack(uint8_t *buffer) {
    struct of15::ofp_meter_multipart_request *mr =
        (struct of15::ofp_meter_multipart_request *) (buffer
            + sizeof(struct of15::ofp_multipart_reply));
    MultipartRequest::unpack(buffer);
    if (this->length_
        < sizeof(struct of15::ofp_multipart_request)
            + sizeof(struct of15::ofp_meter_multipart_request)) {
        return openflow_error(of15::OFPET_BAD_REQUEST, of15::OFPBRC_BAD_LEN);
    }
    this->meter_id_ = ntoh32(mr->meter_id);
    return 0;
}

MultipartReplyMeterConfig::MultipartReplyMeterConfig()
    : MultipartReply(of15::OFPMP_METER_DESC) { //Aman
}

MultipartReplyMeterConfig::MultipartReplyMeterConfig(uint32_t xid,
    uint16_t flags)
    : MultipartReply(xid, of15::OFPMP_METER_DESC, flags) { //Aman
}

MultipartReplyMeterConfig::MultipartReplyMeterConfig(uint32_t xid,
    uint16_t flags, std::vector<MeterConfig> meter_config)
    : MultipartReply(xid, of15::OFPMP_METER_DESC, flags), //Aman
      meter_config_(meter_config) {
    this->length_ += meter_config_len();
}

bool MultipartReplyMeterConfig::operator==(
    const MultipartReplyMeterConfig &other) const {
    return ((MultipartReply::operator==(other))
        && (this->meter_config_ == other.meter_config_));
}

bool MultipartReplyMeterConfig::operator!=(
    const MultipartReplyMeterConfig &other) const {
    return !(*this == other);
}

uint8_t* MultipartReplyMeterConfig::pack() {
    uint8_t* buffer = MultipartReply::pack();
    uint8_t *p = buffer + sizeof(struct of15::ofp_multipart_reply);
    for (std::vector<MeterConfig>::iterator it = this->meter_config_.begin();
        it != this->meter_config_.end(); ++it) {
        it->pack(p);
        p += it->length();
    }
    return buffer;
}

of_error MultipartReplyMeterConfig::unpack(uint8_t *buffer) {
    MultipartReply::unpack(buffer);
    size_t len = this->length_ - sizeof(struct of15::ofp_multipart_reply);
    uint8_t *p = buffer + sizeof(struct of15::ofp_multipart_reply);
    while (len) {
        MeterConfig conf;
        conf.unpack(p);
        this->meter_config_.push_back(conf);
        p += conf.length();
        len -= conf.length();
    }
    return 0;
}

void MultipartReplyMeterConfig::meter_config(
    std::vector<MeterConfig> meter_config) {
    this->meter_config_ = meter_config;
    this->length_ += meter_config_len();
}

void MultipartReplyMeterConfig::add_meter_config(MeterConfig config) {
    this->meter_config_.push_back(config);
    this->length_ += config.length();
}

size_t MultipartReplyMeterConfig::meter_config_len() {
    size_t len;
    for (std::vector<MeterConfig>::iterator it = this->meter_config_.begin();
        it != this->meter_config_.end(); ++it) {
        len += it->length();
    }
    return len;
}

MultipartRequestMeterFeatures::MultipartRequestMeterFeatures()
    : MultipartRequest(OFPMP_METER_FEATURES) {
}

MultipartRequestMeterFeatures::MultipartRequestMeterFeatures(uint32_t xid,
    uint16_t flags)
    : MultipartRequest(xid, of15::OFPMP_METER_FEATURES, flags) {
}

uint8_t* MultipartRequestMeterFeatures::pack() {
    uint8_t* buffer = MultipartRequest::pack();
    return buffer;
}

of_error MultipartRequestMeterFeatures::unpack(uint8_t *buffer) {
    return MultipartRequest::unpack(buffer);
}

MultipartReplyMeterFeatures::MultipartReplyMeterFeatures()
    : MultipartReply(OFPMP_METER_FEATURES) {
}

MultipartReplyMeterFeatures::MultipartReplyMeterFeatures(uint32_t xid,
    uint16_t flags, MeterFeatures features)
    : MultipartReply(xid, of15::OFPMP_METER_FEATURES, flags),
      meter_features_(features) {
    this->meter_features_ = features;
    this->length_ += sizeof(struct of15::ofp_meter_features);
}

bool MultipartReplyMeterFeatures::operator==(
    const MultipartReplyMeterFeatures &other) const {
    return ((MultipartReply::operator==(other))
        && (this->meter_features_ == other.meter_features_));
}

bool MultipartReplyMeterFeatures::operator!=(
    const MultipartReplyMeterFeatures &other) const {
    return !(*this == other);
}

uint8_t* MultipartReplyMeterFeatures::pack() {
    uint8_t* buffer = MultipartReply::pack();
    this->meter_features_.pack(
        buffer + sizeof(struct of15::ofp_multipart_reply));
    return buffer;
}

of_error MultipartReplyMeterFeatures::unpack(uint8_t *buffer) {
    MultipartReply::unpack(buffer);
    this->meter_features_.unpack(
        buffer + sizeof(struct of15::ofp_multipart_reply));
    return 0;
}

MultipartRequestTableFeatures::MultipartRequestTableFeatures()
    : MultipartRequest(OFPMP_TABLE_FEATURES) {
}

MultipartRequestTableFeatures::MultipartRequestTableFeatures(uint32_t xid,
    uint16_t flags)
    : MultipartRequest(xid, of15::OFPMP_TABLE_FEATURES, flags) {
}

MultipartRequestTableFeatures::MultipartRequestTableFeatures(uint32_t xid,
    uint16_t flags, std::vector<TableFeatures> tables_features)
    : MultipartRequest(xid, of15::OFPMP_TABLE_FEATURES, flags) {
    this->tables_features_ = tables_features;
    size_t features_len = 0;
    for (std::vector<TableFeatures>::iterator it =
        this->tables_features_.begin(); it != this->tables_features_.end();
        ++it) {
        features_len += it->length();
    }
    this->length_ += features_len;
}

bool MultipartRequestTableFeatures::operator==(
    const MultipartRequestTableFeatures &other) const {
    return ((MultipartRequest::operator==(other))
        && (this->tables_features_ == other.tables_features_));
}

bool MultipartRequestTableFeatures::operator!=(
    const MultipartRequestTableFeatures &other) const {
    return !(*this == other);
}

uint8_t* MultipartRequestTableFeatures::pack() {
    uint8_t* buffer = MultipartRequest::pack();
    uint8_t *p = buffer + sizeof(struct ofp_multipart_request);
    for (std::vector<TableFeatures>::iterator it =
        this->tables_features_.begin(); it != this->tables_features_.end();
        ++it) {
        it->pack(p);
        p += it->length();
    }
    return buffer;
}

of_error MultipartRequestTableFeatures::unpack(uint8_t *buffer) {
    MultipartRequest::unpack(buffer);
    size_t len = this->length_ - sizeof(struct of15::ofp_multipart_reply);
    uint8_t *p = buffer + sizeof(struct of15::ofp_multipart_reply);
    while (len) {
        TableFeatures features;
        features.unpack(p);
        this->tables_features_.push_back(features);
        p += features.length();
        len -= features.length();
    }
    return 0;
}

void MultipartRequestTableFeatures::tables_features(
    std::vector<TableFeatures> tables_features) {
    this->tables_features_ = tables_features;
    size_t features_len = 0;
    for (std::vector<TableFeatures>::iterator it =
        this->tables_features_.begin(); it != this->tables_features_.end();
        ++it) {
        features_len += it->length();
    }
    this->length_ += features_len;
}

void MultipartRequestTableFeatures::add_table_features(
    TableFeatures table_feature) {
    this->tables_features_.push_back(table_feature);
    this->length_ += table_feature.length();
}

MultipartReplyTableFeatures::MultipartReplyTableFeatures()
    : MultipartReply(OFPMP_TABLE_FEATURES) {
}

MultipartReplyTableFeatures::MultipartReplyTableFeatures(uint32_t xid,
    uint16_t flags)
    : MultipartReply(xid, of15::OFPMP_TABLE_FEATURES, flags) {
}

MultipartReplyTableFeatures::MultipartReplyTableFeatures(uint32_t xid,
    uint16_t flags, std::vector<TableFeatures> tables_features)
    : MultipartReply(xid, of15::OFPMP_TABLE_FEATURES, flags) {
    this->tables_features_ = tables_features;
    size_t features_len = 0;
    for (std::vector<TableFeatures>::iterator it =
        this->tables_features_.begin(); it != this->tables_features_.end();
        ++it) {
        features_len += it->length();
    }
    this->length_ += features_len;
}

bool MultipartReplyTableFeatures::operator==(
    const MultipartReplyTableFeatures &other) const {
    return ((MultipartReply::operator==(other))
        && (this->tables_features_ == other.tables_features_));
}

bool MultipartReplyTableFeatures::operator!=(
    const MultipartReplyTableFeatures &other) const {
    return !(*this == other);
}

uint8_t* MultipartReplyTableFeatures::pack() {
    uint8_t* buffer = MultipartReply::pack();
    uint8_t *p = buffer + sizeof(struct ofp_multipart_request);
    for (std::vector<TableFeatures>::iterator it =
        this->tables_features_.begin(); it != this->tables_features_.end();
        ++it) {
        it->pack(p);
        p += it->length();
    }
    return buffer;
}

of_error MultipartReplyTableFeatures::unpack(uint8_t *buffer) {
    MultipartReply::unpack(buffer);
    size_t len = this->length_ - sizeof(struct of15::ofp_multipart_reply);
    uint8_t *p = buffer + sizeof(struct of15::ofp_multipart_reply);
    while (len) {
        TableFeatures features;
        features.unpack(p);
        this->tables_features_.push_back(features);
        p += features.length();
        len -= features.length();
    }
    return 0;
}

void MultipartReplyTableFeatures::tables_features(
    std::vector<TableFeatures> tables_features) {
    this->tables_features_ = tables_features;
    size_t features_len = 0;
    for (std::vector<TableFeatures>::iterator it =
        this->tables_features_.begin(); it != this->tables_features_.end();
        ++it) {
        features_len += it->length();
    }
    this->length_ += features_len;
}

void MultipartReplyTableFeatures::add_table_features(
    TableFeatures table_feature) {
    this->tables_features_.push_back(table_feature);
    this->length_ += table_feature.length();
}

MultipartRequestPortDescription::MultipartRequestPortDescription()
    : MultipartRequest(OFPMP_PORT_DESC) {
}

MultipartRequestPortDescription::MultipartRequestPortDescription(uint32_t xid,
    uint16_t flags)
    : MultipartRequest(xid, of15::OFPMP_PORT_DESC, flags) {
}

uint8_t* MultipartRequestPortDescription::pack() {
    uint8_t* buffer = MultipartRequest::pack();
    return buffer;
}

of_error MultipartRequestPortDescription::unpack(uint8_t *buffer) {
    return MultipartRequest::unpack(buffer);
}

MultipartReplyPortDescription::MultipartReplyPortDescription()
    : MultipartReply(OFPMP_PORT_DESC) {
}

MultipartReplyPortDescription::MultipartReplyPortDescription(uint32_t xid,
    uint16_t flags)
    : MultipartReply(xid, of15::OFPMP_PORT_DESC, flags) {
}

MultipartReplyPortDescription::MultipartReplyPortDescription(uint32_t xid,
    uint16_t flags, std::vector<of15::Port> ports)
    : MultipartReply(xid, of15::OFPMP_PORT_DESC, flags) {
    this->ports_ = ports;
    this->length_ += ports.size() * sizeof(struct of15::ofp_port);
}

bool MultipartReplyPortDescription::operator==(
    const MultipartReplyPortDescription &other) const {
    return ((MultipartReply::operator==(other))
        && (this->ports_ == other.ports_));
}

bool MultipartReplyPortDescription::operator!=(
    const MultipartReplyPortDescription &other) const {
    return !(*this == other);
}

uint8_t* MultipartReplyPortDescription::pack() {
    uint8_t* buffer = MultipartReply::pack();
    uint8_t *p = buffer + sizeof(struct of15::ofp_multipart_reply);
    for (std::vector<of15::Port>::iterator it = this->ports_.begin();
        it != this->ports_.end(); ++it) {
        it->pack(p);
        p += sizeof(struct of15::ofp_port);
    }
    return buffer;
}

of_error MultipartReplyPortDescription::unpack(uint8_t *buffer) {
    MultipartReply::unpack(buffer);
    uint8_t *p = buffer + sizeof(struct of15::ofp_multipart_reply);
    size_t len = this->length_ - sizeof(struct of15::ofp_multipart_reply);
    while (len) {
        of15::Port port;
        port.unpack(p);
        this->ports_.push_back(port);
        p += sizeof(struct of15::ofp_port);
        len -= sizeof(struct of15::ofp_port);
    }
    return 0;
}

void MultipartReplyPortDescription::ports(std::vector<of15::Port> ports) {
    this->ports_ = ports;
    this->length_ += ports.size() * sizeof(struct of15::ofp_port);
}

void MultipartReplyPortDescription::add_port(of15::Port port) {
    this->ports_.push_back(port);
    this->length_ += sizeof(struct of15::ofp_port);
}

MultipartRequestExperimenter::MultipartRequestExperimenter()
    : MultipartRequest(OFPMP_EXPERIMENTER) {
    this->length_ += sizeof(struct of15::ofp_experimenter_multipart_header);
}

MultipartRequestExperimenter::MultipartRequestExperimenter(uint32_t xid,
    uint16_t flags, uint32_t experimenter, uint32_t exp_type)
    : MultipartRequest(xid, of15::OFPMP_EXPERIMENTER, flags),
      experimenter_(experimenter),
      exp_type_(exp_type) {
    this->length_ += sizeof(struct of15::ofp_experimenter_multipart_header);
}

bool MultipartRequestExperimenter::operator==(
    const MultipartRequestExperimenter &other) const {
    return ((MultipartRequest::operator==(other))
        && (this->experimenter_ == other.experimenter_)
        && (this->exp_type_ == other.exp_type_));
}

bool MultipartRequestExperimenter::operator!=(
    const MultipartRequestExperimenter &other) const {
    return !(*this == other);
}

uint8_t* MultipartRequestExperimenter::pack() {
    uint8_t* buffer = MultipartRequest::pack();
    struct of15::ofp_experimenter_multipart_header *em =
        (struct of15::ofp_experimenter_multipart_header*) buffer;
    em->experimenter = hton32(this->experimenter_);
    em->exp_type = hton32(this->exp_type_);
    return buffer;
}

of_error MultipartRequestExperimenter::unpack(uint8_t *buffer) {
    struct of15::ofp_experimenter_multipart_header *em =
        (struct of15::ofp_experimenter_multipart_header*) buffer;
    MultipartRequest::unpack(buffer);
    this->experimenter_ = ntoh32(em->experimenter);
    this->exp_type_ = ntoh32(em->exp_type);
    return 0;
}

MultipartReplyExperimenter::MultipartReplyExperimenter()
    : MultipartReply(OFPMP_EXPERIMENTER) {
    this->length_ += sizeof(struct of15::ofp_experimenter_multipart_header);
}

MultipartReplyExperimenter::MultipartReplyExperimenter(uint32_t xid,
    uint16_t flags, uint32_t experimenter, uint32_t exp_type)
    : MultipartReply(xid, of15::OFPMP_EXPERIMENTER, flags),
      experimenter_(experimenter),
      exp_type_(exp_type) {
    this->length_ += sizeof(struct of15::ofp_experimenter_multipart_header);
}

bool MultipartReplyExperimenter::operator==(
    const MultipartReplyExperimenter &other) const {
    return ((MultipartReply::operator==(other))
        && (this->experimenter_ == other.experimenter_)
        && (this->exp_type_ == other.exp_type_));
}

bool MultipartReplyExperimenter::operator!=(
    const MultipartReplyExperimenter &other) const {
    return !(*this == other);
}

uint8_t* MultipartReplyExperimenter::pack() {
    uint8_t* buffer = MultipartReply::pack();
    struct of15::ofp_experimenter_multipart_header *em =
        (struct of15::ofp_experimenter_multipart_header*) buffer;
    em->experimenter = hton32(this->experimenter_);
    em->exp_type = hton32(this->exp_type_);
    return buffer;
}

of_error MultipartReplyExperimenter::unpack(uint8_t *buffer) {
    struct of15::ofp_experimenter_multipart_header *em =
        (struct of15::ofp_experimenter_multipart_header*) buffer;
    MultipartReply::unpack(buffer);
    if (this->length_
        < sizeof(struct of15::ofp_multipart_request)
            + sizeof(struct of15::ofp_experimenter_multipart_header)) {
        return openflow_error(of15::OFPET_BAD_REQUEST, of15::OFPBRC_BAD_LEN);
    }
    this->experimenter_ = ntoh32(em->experimenter);
    this->exp_type_ = ntoh32(em->exp_type);
    return 0;
}

//Aman
// TODO: Change to multipart request
//QueueGetConfigRequest::QueueGetConfigRequest()
//    : OFMsg(of15::OFP_VERSION, of15::OFPT_QUEUE_GET_CONFIG_REQUEST) {
//    this->length_ = sizeof(struct of15::ofp_queue_get_config_request);
//}
//
//QueueGetConfigRequest::QueueGetConfigRequest(uint32_t xid, uint32_t port)
//    : OFMsg(of15::OFP_VERSION, of15::OFPT_QUEUE_GET_CONFIG_REQUEST, xid),
//      port_(port) {
//    this->length_ = sizeof(struct of15::ofp_queue_get_config_request);
//}
//
//bool QueueGetConfigRequest::operator==(
//    const QueueGetConfigRequest &other) const {
//    return ((OFMsg::operator==(other)) && (this->port_ == other.port_));
//}
//
//bool QueueGetConfigRequest::operator!=(
//    const QueueGetConfigRequest &other) const {
//    return !(*this == other);
//}
//
//uint8_t* QueueGetConfigRequest::pack() {
//    uint8_t* buffer = OFMsg::pack();
//    struct of15::ofp_queue_get_config_request * qc =
//        (struct of15::ofp_queue_get_config_request*) buffer;
//    qc->port = hton32(this->port_);
//    memset(qc->pad, 0x0, 4);
//    return buffer;
//}
//
//of_error QueueGetConfigRequest::unpack(uint8_t *buffer) {
//    struct of15::ofp_queue_get_config_request * qc =
//        (struct of15::ofp_queue_get_config_request*) buffer;
//    OFMsg::unpack(buffer);
//    if (this->length_ < sizeof(struct of15::ofp_queue_get_config_request)) {
//        return openflow_error(of15::OFPET_BAD_REQUEST, of15::OFPBRC_BAD_LEN);
//    }
//    this->port_ = ntoh32(qc->port);
//    return 0;
//}
//
//QueueGetConfigReply::QueueGetConfigReply()
//    : OFMsg(of15::OFP_VERSION, of15::OFPT_QUEUE_GET_CONFIG_REPLY) {
//    this->length_ = sizeof(struct of15::ofp_queue_get_config_reply);
//}
//
//QueueGetConfigReply::QueueGetConfigReply(uint32_t xid, uint32_t port)
//    : OFMsg(of15::OFP_VERSION, of15::OFPT_QUEUE_GET_CONFIG_REPLY, xid) {
//    this->port_ = port;
//    this->length_ = sizeof(struct of15::ofp_queue_get_config_reply);
//}
//
//QueueGetConfigReply::QueueGetConfigReply(uint32_t xid, uint16_t port,
//    std::list<of15::PacketQueue> queues)
//    : OFMsg(of15::OFP_VERSION, of15::OFPT_QUEUE_GET_CONFIG_REPLY, xid),
//      port_(port),
//      queues_(queues) {
//    this->length_ = sizeof(struct of15::ofp_queue_get_config_reply)
//        + queues_len();
//}
//
//bool QueueGetConfigReply::operator==(const QueueGetConfigReply &other) const {
//    return ((OFMsg::operator==(other)) && (this->port_ == other.port_)
//        && (this->queues_ == other.queues_));
//}
//
//bool QueueGetConfigReply::operator!=(const QueueGetConfigReply &other) const {
//    return !(*this == other);
//}
//
//uint8_t* QueueGetConfigReply::pack() {
//    uint8_t* buffer = OFMsg::pack();
//    struct of15::ofp_queue_get_config_reply *qr =
//        (struct of15::ofp_queue_get_config_reply *) buffer;
//    qr->port = hton32(this->port_);
//    memset(qr->pad, 0x0, 6);
//    uint8_t *p = buffer + sizeof(struct of15::ofp_queue_get_config_reply);
//    for (std::list<PacketQueue>::iterator it = this->queues_.begin(), end =
//        this->queues_.end(); it != end; ++it) {
//        it->pack(p);
//        p += it->len();
//    }
//    return buffer;
//}
//
//of_error QueueGetConfigReply::unpack(uint8_t *buffer) {
//    struct of15::ofp_queue_get_config_reply *qr =
//        (struct of15::ofp_queue_get_config_reply *) buffer;
//    OFMsg::unpack(buffer);
//    this->port_ = ntoh32(qr->port);
//    uint8_t *p = buffer + sizeof(struct of15::ofp_queue_get_config_reply);
//    size_t len = this->length_
//        - sizeof(struct of15::ofp_queue_get_config_reply);
//    while (len) {
//        PacketQueue pq;
//        pq.unpack(p);
//        this->queues_.push_back(pq);
//        p += pq.len();
//        len -= pq.len();
//    }
//    return 0;
//}
//
//void QueueGetConfigReply::queues(std::list<of15::PacketQueue> queues) {
//    this->queues_ = queues;
//    this->length_ += queues_len();
//}
//
//void QueueGetConfigReply::add_queue(PacketQueue queue) {
//    this->queues_.push_back(queue);
//    this->length_ += queue.len();
//}
//
//size_t QueueGetConfigReply::queues_len() {
//    size_t len;
//    for (std::list<PacketQueue>::iterator it = this->queues_.begin(), end =
//        this->queues_.end(); it != end; ++it) {
//        len += it->len();
//    }
//    return len;
//}

BarrierRequest::BarrierRequest()
    : OFMsg(of15::OFP_VERSION, of15::OFPT_BARRIER_REQUEST) {
}

BarrierRequest::BarrierRequest(uint32_t xid)
    : OFMsg(of15::OFP_VERSION, of15::OFPT_BARRIER_REQUEST, xid) {
}

uint8_t* BarrierRequest::pack() {
    return OFMsg::pack();
}

of_error BarrierRequest::unpack(uint8_t *buffer) {
    OFMsg::unpack(buffer);
    if (this->length_ < sizeof(struct ofp_header)) {
        return openflow_error(of15::OFPET_BAD_REQUEST, of15::OFPBRC_BAD_LEN);
    }
    return 0;
}

BarrierReply::BarrierReply()
    : OFMsg(of15::OFP_VERSION, of15::OFPT_BARRIER_REPLY) {
}

BarrierReply::BarrierReply(uint32_t xid)
    : OFMsg(of15::OFP_VERSION, of15::OFPT_BARRIER_REPLY, xid) {
}

uint8_t* BarrierReply::pack() {
    return OFMsg::pack();
}

of_error BarrierReply::unpack(uint8_t *buffer) {
    OFMsg::unpack(buffer);
    return 0;
}

RoleRequest::RoleRequest()
    : RoleCommon(of15::OFP_VERSION, of15::OFPT_ROLE_REQUEST) {
}

RoleRequest::RoleRequest(uint32_t xid, uint32_t role, uint64_t generation_id)
    : RoleCommon(of15::OFP_VERSION, of15::OFPT_ROLE_REQUEST, xid, role,
        generation_id) {
}

RoleReply::RoleReply()
    : RoleCommon(of15::OFP_VERSION, of15::OFPT_ROLE_REPLY) {
}

RoleReply::RoleReply(uint32_t xid, uint32_t role, uint64_t generation_id)
    : RoleCommon(of15::OFP_VERSION, of15::OFPT_ROLE_REPLY, xid, role,
        generation_id) {
}

GetAsyncRequest::GetAsyncRequest()
    : OFMsg(of15::OFP_VERSION, of15::OFPT_GET_ASYNC_REQUEST) {
}

GetAsyncRequest::GetAsyncRequest(uint32_t xid)
    : OFMsg(of15::OFP_VERSION, of15::OFPT_GET_ASYNC_REQUEST, xid) {
}

uint8_t* GetAsyncRequest::pack() {
    return OFMsg::pack();
}

of_error GetAsyncRequest::unpack(uint8_t *buffer) {
    OFMsg::unpack(buffer);
    if (this->length_ < sizeof(struct ofp_header)) {
        return openflow_error(of15::OFPET_BAD_REQUEST, of15::OFPBRC_BAD_LEN);
    }
    return 0;
}

GetAsyncReply::GetAsyncReply()
    : AsyncConfigCommon(of15::OFP_VERSION, of15::OFPT_GET_ASYNC_REPLY) {
}

GetAsyncReply::GetAsyncReply(uint32_t xid)
    : AsyncConfigCommon(of15::OFP_VERSION, of15::OFPT_GET_ASYNC_REPLY, xid) {
}

GetAsyncReply::GetAsyncReply(uint32_t xid, std::vector<uint32_t> packet_in_mask,
    std::vector<uint32_t> port_status_mask,
    std::vector<uint32_t> flow_removed_mask)
    : AsyncConfigCommon(of15::OFP_VERSION, of15::OFPT_GET_ASYNC_REPLY, xid,
        packet_in_mask, port_status_mask, flow_removed_mask) {

}

SetAsync::SetAsync()
    : AsyncConfigCommon(of15::OFP_VERSION, of15::OFPT_SET_ASYNC) {
}

SetAsync::SetAsync(uint32_t xid)
    : AsyncConfigCommon(of15::OFP_VERSION, of15::OFPT_SET_ASYNC, xid) {
}

SetAsync::SetAsync(uint32_t xid, std::vector<uint32_t> packet_in_mask,
    std::vector<uint32_t> port_status_mask,
    std::vector<uint32_t> flow_removed_mask)
    : AsyncConfigCommon(of15::OFP_VERSION, of15::OFPT_SET_ASYNC, xid,
        packet_in_mask, port_status_mask, flow_removed_mask) {

}

MeterMod::MeterMod()
    : OFMsg(of15::OFP_VERSION, of15::OFPT_METER_MOD) {
    this->length_ = sizeof(struct of15::ofp_meter_mod);
}

MeterMod::MeterMod(uint32_t xid, uint16_t command, uint16_t flags,
    uint32_t meter_id)
    : OFMsg(of15::OFP_VERSION, of15::OFPT_METER_MOD, xid),
      command_(command),
      meter_id_(meter_id),
      flags_(flags) {
    this->length_ = sizeof(struct of15::ofp_meter_mod);
}

MeterMod::MeterMod(uint32_t xid, uint16_t command, uint16_t flags,
    uint32_t meter_id, MeterBandList bands)
    : OFMsg(of15::OFP_VERSION, of15::OFPT_METER_MOD, xid),
      command_(command),
      meter_id_(meter_id),
      flags_(flags),
      bands_(bands) {
    this->length_ = sizeof(struct of15::ofp_meter_mod) + bands.length();
}

bool MeterMod::operator==(const MeterMod &other) const {
    return ((OFMsg::operator==(other)) && (this->command_ == other.command_)
        && (this->flags_ == other.flags_)
        && (this->meter_id_ == other.meter_id_)
        && (this->bands_ == other.bands_));
}

bool MeterMod::operator!=(const MeterMod &other) const {
    return !(*this == other);
}

uint8_t* MeterMod::pack() {
    uint8_t* buffer = OFMsg::pack();
    struct of15::ofp_meter_mod *mm = (struct of15::ofp_meter_mod *) buffer;
    mm->command = hton16(this->command_);
    mm->flags = hton16(this->flags_);
    mm->meter_id = hton32(this->meter_id_);
    uint8_t *p = buffer + sizeof(struct of15::ofp_meter_mod);
    this->bands_.pack(p);
    return buffer;
}

of_error MeterMod::unpack(uint8_t *buffer) {
    struct of15::ofp_meter_mod *mm = (struct of15::ofp_meter_mod *) buffer;
    OFMsg::unpack(buffer);
    if (this->length_ < sizeof(of15::ofp_meter_mod)) {
        return openflow_error(of15::OFPET_BAD_REQUEST, of15::OFPBRC_BAD_LEN);
    }
    this->command_ = ntoh16(mm->command);
    this->flags_ = ntoh16(mm->flags);
    this->meter_id_ = ntoh32(mm->meter_id);
    uint8_t *p = buffer + sizeof(struct of15::ofp_meter_mod);
    this->bands_.length(this->length_ - sizeof(struct of15::ofp_meter_mod));
    this->bands_.unpack(p);
    return 0;
}

void MeterMod::bands(MeterBandList bands) {
    this->bands_ = bands;
    this->length_ += bands.length();
}

void MeterMod::add_band(MeterBand* band) {
    this->bands_.add_band(band);
    this->length_ += band->len();
}

} // End of namespace of15
} //End of namespace fluid_msg

