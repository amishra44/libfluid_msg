#include "of15action.hh"
#include <stdio.h>
#include <iostream>

namespace fluid_msg {

namespace of15 {

OutputAction::OutputAction()
    : set_order_(230),
      Action(of15::OFPAT_OUTPUT, sizeof(struct of15::ofp_action_output)) {
}

bool OutputAction::equals(const Action &other) {

    if (const OutputAction * act = dynamic_cast<const OutputAction *>(&other)) {
        return ((Action::equals(other)) && (this->port_ == act->port_)
            && (this->max_len_ == act->max_len_));
    }
    else {
        return false;
    }
}

OutputAction::OutputAction(uint32_t port, uint16_t max_len)
    : set_order_(230),
      Action(of15::OFPAT_OUTPUT, sizeof(struct of15::ofp_action_output)) {
    this->port_ = port;
    this->max_len_ = max_len;
}

size_t OutputAction::pack(uint8_t* buffer) {
    struct of15::ofp_action_output* ao =
        (struct of15::ofp_action_output*) buffer;
    Action::pack(buffer);
    ao->port = hton32(this->port_);
    ao->max_len = hton16(this->max_len_);
    memset(ao->pad, 0x0, 6);
    return 0;
}

of_error OutputAction::unpack(uint8_t *buffer) {
    struct of15::ofp_action_output* ao =
        (struct of15::ofp_action_output*) buffer;
    Action::unpack(buffer);
    this->port_ = ntoh32(ao->port);
    this->max_len_ = ntoh16(ao->max_len);
    return 0;
}

CopyTTLInAction::CopyTTLInAction()
    : set_order_(10),
      Action(of15::OFPAT_COPY_TTL_IN, sizeof(struct ofp_action_header)) {
}

size_t CopyTTLInAction::pack(uint8_t* buffer) {
    struct ofp_action_header * act = (struct ofp_action_header*) buffer;
    Action::pack(buffer);
    memset(act->pad, 0x0, 4);
    return 0;
}

of_error CopyTTLInAction::unpack(uint8_t *buffer) {
    Action::unpack(buffer);
    return 0;
}

CopyTTLOutAction::CopyTTLOutAction()
    : set_order_(110),
      Action(of15::OFPAT_COPY_TTL_OUT, sizeof(struct ofp_action_header)) {
}

size_t CopyTTLOutAction::pack(uint8_t* buffer) {
    struct ofp_action_header * act = (struct ofp_action_header*) buffer;
    Action::pack(buffer);
    memset(act->pad, 0x0, 4);
    return 0;
}

of_error CopyTTLOutAction::unpack(uint8_t *buffer) {
    Action::unpack(buffer);
    return 0;
}

SetMPLSTTLAction::SetMPLSTTLAction()
    : set_order_(140),
      Action(of15::OFPAT_SET_MPLS_TTL, sizeof(struct of15::ofp_action_mpls_ttl)) {
}

SetMPLSTTLAction::SetMPLSTTLAction(uint8_t mpls_ttl)
    : set_order_(140),
      Action(of15::OFPAT_SET_MPLS_TTL, sizeof(struct of15::ofp_action_mpls_ttl)) {
    this->mpls_ttl_ = mpls_ttl;
}

bool SetMPLSTTLAction::equals(const Action &other) {

    if (const SetMPLSTTLAction * act =
        dynamic_cast<const SetMPLSTTLAction *>(&other)) {
        return ((Action::equals(other)) && (this->mpls_ttl_ == act->mpls_ttl_));
    }
    else {
        return false;
    }
}

size_t SetMPLSTTLAction::pack(uint8_t* buffer) {
    struct of15::ofp_action_mpls_ttl * mt =
        (struct of15::ofp_action_mpls_ttl*) buffer;
    Action::pack(buffer);
    mt->mpls_ttl = this->mpls_ttl_;
    memset(mt->pad, 0x0, 3);
    return 0;
}

of_error SetMPLSTTLAction::unpack(uint8_t *buffer) {
    struct of15::ofp_action_mpls_ttl * mt =
        (struct of15::ofp_action_mpls_ttl*) buffer;
    Action::unpack(buffer);
    this->mpls_ttl_ = mt->mpls_ttl;
    return 0;
}

DecMPLSTTLAction::DecMPLSTTLAction()
    : set_order_(120),
      Action(of15::OFPAT_DEC_MPLS_TTL, sizeof(struct ofp_action_header)) {
}

size_t DecMPLSTTLAction::pack(uint8_t* buffer) {
    struct ofp_action_header * act = (struct ofp_action_header*) buffer;
    Action::pack(buffer);
    memset(act->pad, 0x0, 4);
    return 0;
}

of_error DecMPLSTTLAction::unpack(uint8_t *buffer) {
    Action::unpack(buffer);
    return 0;
}

PushVLANAction::PushVLANAction()
    : set_order_(100),
      Action(of15::OFPAT_PUSH_VLAN, sizeof(struct of15::ofp_action_push)) {
}

PushVLANAction::PushVLANAction(uint16_t ethertype)
    : set_order_(100),
      Action(of15::OFPAT_PUSH_VLAN, sizeof(struct of15::ofp_action_push)) {
    this->ethertype_ = ethertype;
}

bool PushVLANAction::equals(const Action &other) {

    if (const PushVLANAction * act =
        dynamic_cast<const PushVLANAction *>(&other)) {
        return ((Action::equals(other)) && (this->ethertype_ == act->ethertype_));
    }
    else {
        return false;
    }
}

size_t PushVLANAction::pack(uint8_t* buffer) {
    struct of15::ofp_action_push *ap = (struct of15::ofp_action_push*) buffer;
    Action::pack(buffer);
    ap->ethertype = hton16(this->ethertype_);
    memset(ap->pad, 0x0, 2);
    return 0;
}

of_error PushVLANAction::unpack(uint8_t *buffer) {
    struct of15::ofp_action_push *ap = (struct of15::ofp_action_push*) buffer;
    Action::unpack(buffer);
    this->ethertype_ = ntoh16(ap->ethertype);
    return 0;
}

PopVLANAction::PopVLANAction()
    : set_order_(30),
      Action(of15::OFPAT_POP_VLAN, sizeof(struct ofp_action_header)) {
}

size_t PopVLANAction::pack(uint8_t* buffer) {
    struct ofp_action_header * act = (struct ofp_action_header*) buffer;
    Action::pack(buffer);
    memset(act->pad, 0x0, 4);
    return 0;
}

of_error PopVLANAction::unpack(uint8_t *buffer) {
    Action::unpack(buffer);
    return 0;
}

PushMPLSAction::PushMPLSAction()
    : set_order_(80),
      Action(of15::OFPAT_PUSH_MPLS, sizeof(struct ofp_action_header)) {
}

PushMPLSAction::PushMPLSAction(uint16_t ethertype)
    : set_order_(80),
      Action(of15::OFPAT_PUSH_MPLS, sizeof(struct ofp_action_header)) {
    this->ethertype_ = ethertype;
}

bool PushMPLSAction::equals(const Action &other) {

    if (const PushMPLSAction * act =
        dynamic_cast<const PushMPLSAction *>(&other)) {
        return ((Action::equals(other)) && (this->ethertype_ == act->ethertype_));
    }
    else {
        return false;
    }
}

size_t PushMPLSAction::pack(uint8_t* buffer) {
    struct of15::ofp_action_push *ap = (struct of15::ofp_action_push*) buffer;
    Action::pack(buffer);
    ap->ethertype = hton16(this->ethertype_);
    memset(ap->pad, 0x0, 2);
    return 0;
}

of_error PushMPLSAction::unpack(uint8_t *buffer) {
    struct of15::ofp_action_push *ap = (struct of15::ofp_action_push*) buffer;
    Action::unpack(buffer);
    this->ethertype_ = ntoh16(ap->ethertype);
    return 0;
}

PopMPLSAction::PopMPLSAction()
    : set_order_(40),
      Action(of15::OFPAT_POP_MPLS, sizeof(struct of15::ofp_action_pop_mpls)) {
}

PopMPLSAction::PopMPLSAction(uint16_t ethertype)
    : set_order_(40),
      Action(of15::OFPAT_POP_MPLS, sizeof(struct of15::ofp_action_pop_mpls)) {
    this->ethertype_ = ethertype;
}

bool PopMPLSAction::equals(const Action &other) {

    if (const PopMPLSAction * act = dynamic_cast<const PopMPLSAction *>(&other)) {
        return ((Action::equals(other)) && (this->ethertype_ == act->ethertype_));
    }
    else {
        return false;
    }
}

size_t PopMPLSAction::pack(uint8_t* buffer) {
    struct of15::ofp_action_pop_mpls *pm =
        (struct of15::ofp_action_pop_mpls*) buffer;
    Action::pack(buffer);
    pm->ethertype = hton16(this->ethertype_);
    memset(pm->pad, 0x0, 2);
    return 0;
}

of_error PopMPLSAction::unpack(uint8_t *buffer) {
    struct of15::ofp_action_pop_mpls *pm =
        (struct of15::ofp_action_pop_mpls*) buffer;
    Action::unpack(buffer);
    this->ethertype_ = ntoh16(pm->ethertype);
    return 0;
}

SetQueueAction::SetQueueAction()
    : set_order_(170),
      Action(of15::OFPAT_SET_QUEUE, sizeof(struct of15::ofp_action_set_queue)) {
}

SetQueueAction::SetQueueAction(uint32_t queue_id)
    : set_order_(170),
      Action(of15::OFPAT_SET_QUEUE, sizeof(struct of15::ofp_action_set_queue)) {
    this->queue_id_ = queue_id;
}

bool SetQueueAction::equals(const Action &other) {

    if (const SetQueueAction * act =
        dynamic_cast<const SetQueueAction *>(&other)) {
        return ((Action::equals(other)) && (this->queue_id_ == act->queue_id_));
    }
    else {
        return false;
    }
}

size_t SetQueueAction::pack(uint8_t* buffer) {
    struct of15::ofp_action_set_queue* aq =
        (struct of15::ofp_action_set_queue*) buffer;
    Action::pack(buffer);
    aq->queue_id = hton32(this->queue_id_);
    return 0;
}

of_error SetQueueAction::unpack(uint8_t *buffer) {
    struct of15::ofp_action_set_queue* aq =
        (struct of15::ofp_action_set_queue*) buffer;
    Action::unpack(buffer);
    this->queue_id_ = ntoh32(aq->queue_id);
    return 0;
}

GroupAction::GroupAction()
    : set_order_(220),
      Action(of15::OFPAT_GROUP, sizeof(struct of15::ofp_action_group)) {
}

GroupAction::GroupAction(uint32_t group_id)
    : set_order_(220),
      Action(of15::OFPAT_GROUP, sizeof(struct of15::ofp_action_group)) {
    this->group_id_ = group_id;
}

bool GroupAction::equals(const Action &other) {

    if (const GroupAction * act = dynamic_cast<const GroupAction *>(&other)) {
        return ((Action::equals(other)) && (this->group_id_ == act->group_id_));
    }
    else {
        return false;
    }
}

size_t GroupAction::pack(uint8_t* buffer) {
    struct of15::ofp_action_group *ag = (struct of15::ofp_action_group*) buffer;
    Action::pack(buffer);
    ag->group_id = hton32(this->group_id_);
    return 0;
}

of_error GroupAction::unpack(uint8_t *buffer) {
    struct ofp_action_group *ag = (struct ofp_action_group*) buffer;
    Action::unpack(buffer);
    this->group_id_ = ntoh32(ag->group_id);
    return 0;
}

SetNWTTLAction::SetNWTTLAction()
    : set_order_(150),
      Action(of15::OFPAT_SET_NW_TTL, sizeof(struct of15::ofp_action_nw_ttl)) {
}

SetNWTTLAction::SetNWTTLAction(uint8_t nw_ttl)
    : set_order_(150),
      Action(of15::OFPAT_SET_NW_TTL, sizeof(struct of15::ofp_action_nw_ttl)) {
    this->nw_ttl_ = nw_ttl;
}

bool SetNWTTLAction::equals(const Action &other) {

    if (const SetNWTTLAction * act =
        dynamic_cast<const SetNWTTLAction *>(&other)) {
        return ((Action::equals(other)) && (this->nw_ttl_ == act->nw_ttl_));
    }
    else {
        return false;
    }
}

size_t SetNWTTLAction::pack(uint8_t* buffer) {
    struct of15::ofp_action_nw_ttl *nt =
        (struct of15::ofp_action_nw_ttl*) buffer;
    Action::pack(buffer);
    nt->nw_ttl = this->nw_ttl_;
    memset(nt->pad, 0x0, 3);
    return 0;
}

of_error SetNWTTLAction::unpack(uint8_t *buffer) {
    struct of15::ofp_action_nw_ttl *nt =
        (struct of15::ofp_action_nw_ttl*) buffer;
    Action::unpack(buffer);
    this->nw_ttl_ = nt->nw_ttl;
    return 0;
}

DecNWTTLAction::DecNWTTLAction()
    : set_order_(130),
      Action(of15::OFPAT_DEC_NW_TTL, sizeof(struct ofp_action_header)) {
}

size_t DecNWTTLAction::pack(uint8_t* buffer) {
    struct ofp_action_header * act = (struct ofp_action_header*) buffer;
    Action::pack(buffer);
    memset(act->pad, 0x0, 4);
    return 0;
}

of_error DecNWTTLAction::unpack(uint8_t *buffer) {
    Action::unpack(buffer);
    return 0;
}

SetFieldAction::SetFieldAction()
    : field_(0),
      set_order_(160),
      Action(of15::OFPAT_SET_FIELD, sizeof(struct of15::ofp_action_set_field)) {

}

SetFieldAction::SetFieldAction(OXMTLV* field)
    : field_(0),
      set_order_(160),
      Action(of15::OFPAT_SET_FIELD, sizeof(struct of15::ofp_action_set_field)) {
    this->field(field);
}

SetFieldAction::SetFieldAction(const SetFieldAction &other)
    : set_order_(160) {
    this->type_ = other.type_;
    this->length_ = other.length_;
    this->field_ = other.field_->clone();
}

void SetFieldAction::field(OXMTLV* field) {
    this->field_ = field;
    this->length_ += ROUND_UP(field->length(), 8);
}

SetFieldAction::~SetFieldAction() {
    delete this->field_;
}

void swap(SetFieldAction& first, SetFieldAction& second) {
    std::swap(first.type_, second.type_);
    std::swap(first.length_, second.length_);
    std::swap(*(first.field_), *(second.field_));
}

SetFieldAction& SetFieldAction::operator=(SetFieldAction other) {
    swap(*this, other);
    return *this;
}

bool SetFieldAction::equals(const Action &other) {
    const SetFieldAction * action;
    if (const SetFieldAction * act =
        dynamic_cast<const SetFieldAction *>(&other)) {
        return ((Action::equals(other)) && (this->field_->equals(*act->field_))
            && (this->length_ == act->length_));
    }
    else {
        return false;
    }
}

OXMTLV* SetFieldAction::field() {
    return this->field_;
}

size_t SetFieldAction::pack(uint8_t* buffer) {
    size_t padding = ROUND_UP(this->field_->length(), 8)
        - this->field_->length();
    Action::pack(buffer);
    this->field_->pack(
        buffer + (sizeof(struct of15::ofp_action_set_field) - 4));
    memset(
        buffer + sizeof(struct of15::ofp_action_set_field)
            + this->field_->length(), 0x0, padding);
    return 0;
}

of_error SetFieldAction::unpack(uint8_t *buffer) {
    uint8_t * p = buffer + sizeof(struct of15::ofp_action_set_field) - 4;
    size_t padding;
    Action::unpack(buffer);
    uint32_t header = ntoh32(*((uint32_t*) p));
//    std::cout << "SetField class: " << OXMTLV::oxm_class(header);

    if(OXMTLV::oxm_class(header) == fluid_msg::of15::OFPXMC_PACKET_REGS) { //Aman
        this->field_ = new of15::PacketReg(OXMTLV::oxm_field(header));
//        std::cout << "SetField packet reg #: " << this->field()->field();
//        std::cout << "SetField packet reg value: " << ((PacketReg *)this->field())->value();
    } else {
        this->field_ = of15::Match::make_oxm_tlv(this->field_->oxm_field(header));
    }
    this->field_->unpack(p);
    return 0;
}

PushPBBAction::PushPBBAction()
    : set_order_(90),
      Action(of15::OFPAT_PUSH_PBB, sizeof(struct of15::ofp_action_push)) {
}

PushPBBAction::PushPBBAction(uint16_t ethertype)
    : set_order_(90),
      Action(of15::OFPAT_PUSH_PBB, sizeof(struct of15::ofp_action_push)) {
    this->ethertype_ = ethertype;
}

bool PushPBBAction::equals(const Action &other) {

    if (const PushPBBAction * act = dynamic_cast<const PushPBBAction *>(&other)) {
        return ((Action::equals(other)) && (this->ethertype_ == act->ethertype_));
    }
    else {
        return false;
    }
}

size_t PushPBBAction::pack(uint8_t* buffer) {
    struct of15::ofp_action_push *ap = (struct of15::ofp_action_push*) buffer;
    Action::pack(buffer);
    ap->ethertype = hton16(this->ethertype_);
    memset(ap->pad, 0x0, 2);
    return 0;
}

of_error PushPBBAction::unpack(uint8_t *buffer) {
    struct of15::ofp_action_push *ap = (struct of15::ofp_action_push*) buffer;
    Action::unpack(buffer);
    this->ethertype_ = ntoh16(ap->ethertype);
    return 0;
}

PopPBBAction::PopPBBAction()
    : set_order_(50),
      Action(of15::OFPAT_POP_PBB, sizeof(struct of15::ofp_action_push)) {
}

size_t PopPBBAction::pack(uint8_t* buffer) {
    struct ofp_action_header * act = (struct ofp_action_header*) buffer;
    Action::pack(buffer);
    memset(act->pad, 0x0, 4);
    return 0;
}

of_error PopPBBAction::unpack(uint8_t *buffer) {
    Action::unpack(buffer);
    return 0;
}

CopyFieldAction::CopyFieldAction()
    : n_bits_(0),
      src_offset_(0),
      dst_offset_(0),
      Action(of15::OFPAT_COPY_FIELD, sizeof(struct of15::ofp_action_copy_field)) {
}

CopyFieldAction::CopyFieldAction(uint16_t n_bits, uint16_t src_offset, uint16_t dst_offset, std::vector<uint32_t> oxm_ids)
    : n_bits_(n_bits),
      src_offset_(src_offset),
      dst_offset_(dst_offset),
      Action(of15::OFPAT_SET_FIELD, sizeof(struct of15::ofp_action_set_field)) {
    this->oxm_ids_ = oxm_ids;
    this->length_ += oxm_ids_.size() * sizeof(uint32_t);
}

CopyFieldAction::CopyFieldAction(const CopyFieldAction &other) {
    this->type_ = other.type_;
    this->length_ = other.length_;
    this->n_bits_ = other.n_bits_;
    this->src_offset_ = other.src_offset_;
    this->dst_offset_ = other.dst_offset_;
    this->oxm_ids_ = other.oxm_ids_;
}

void swap(CopyFieldAction& first, CopyFieldAction& second) {
    std::swap(first.type_, second.type_);
    std::swap(first.length_, second.length_);
    std::swap(first.n_bits_, second.n_bits_);
    std::swap(first.src_offset_, second.src_offset_);
    std::swap(first.dst_offset_, second.dst_offset_);
    std::swap(first.oxm_ids_, second.oxm_ids_);
}

CopyFieldAction& CopyFieldAction::operator=(CopyFieldAction other) {
    swap(*this, other);
    return *this;
}

bool CopyFieldAction::equals(const Action &other) {
    const CopyFieldAction * action;
    if (const CopyFieldAction * act =
        dynamic_cast<const CopyFieldAction *>(&other)) {
        return ((Action::equals(other))
            && (this->length_ == act->length_)
            && (this->n_bits_ == act->n_bits_)
            && (this->src_offset_ == act->src_offset_)
            && (this->dst_offset_ == act->dst_offset_)
            && (this->oxm_ids_ == act->oxm_ids_));
    }
    else {
        return false;
    }
}

size_t CopyFieldAction::pack(uint8_t* buffer) {
    Action::pack(buffer);
    struct of15::ofp_action_copy_field * cf = (struct of15::ofp_action_copy_field *)buffer;
    cf->n_bits = this->n_bits_;
    cf->src_offset = this->src_offset_;
    cf->dst_offset = this->dst_offset_;
    memset(cf->pad, 0x0, 2);
    uint8_t * p = buffer + sizeof(struct of15::ofp_action_copy_field);
    for (std::vector<uint32_t>::iterator it = this->oxm_ids_.begin();
        it != this->oxm_ids_.end(); ++it) {
        memcpy(p, &(*it), sizeof(uint32_t));
        p += sizeof(uint32_t);
    }
    return length();
}

of_error CopyFieldAction::unpack(uint8_t *buffer) {
    Action::unpack(buffer);
    struct of15::ofp_action_copy_field * cf = (struct of15::ofp_action_copy_field *)buffer;
    this->n_bits_ = ntoh16(cf->n_bits);
    this->src_offset_ = ntoh16(cf->src_offset);
    this->dst_offset_ = ntoh16(cf->dst_offset);

    uint8_t * p = buffer + sizeof(struct of15::ofp_action_copy_field);
    size_t len = this->length_
        - sizeof(struct of15::ofp_action_copy_field);
    while (len) {
        uint32_t oxm_id = ntoh32(*((uint32_t*) p));

        this->oxm_ids_.push_back(oxm_id);
        p += sizeof(uint32_t);
        len -= sizeof(uint32_t);
    }
    return 0;
}

void CopyFieldAction::oxm_ids(std::vector<uint32_t> oxm_ids) {
    this->oxm_ids_ = oxm_ids;
    this->length_ += oxm_ids_.size() * sizeof(uint32_t);
}

ExperimenterAction::ExperimenterAction()
    : Action(of15::OFPAT_EXPERIMENTER,
        sizeof(struct of15::ofp_action_experimenter_header)) {
}

ExperimenterAction::ExperimenterAction(uint32_t experimenter)
    : Action(of15::OFPAT_EXPERIMENTER,
        sizeof(struct of15::ofp_action_experimenter_header)) {
    this->experimenter_ = experimenter;
}

bool ExperimenterAction::equals(const Action &other) {

    if (const ExperimenterAction * act =
        dynamic_cast<const ExperimenterAction *>(&other)) {
        return ((Action::equals(other))
            && (this->experimenter_ == act->experimenter_));
    }
    else {
        return false;
    }
}

size_t ExperimenterAction::pack(uint8_t* buffer) {
    struct ofp_action_experimenter_header * ae =
        (struct ofp_action_experimenter_header*) buffer;
    Action::pack(buffer);
    ae->experimenter = hton32(this->experimenter_);
    return 0;
}

of_error ExperimenterAction::unpack(uint8_t *buffer) {
    struct ofp_action_experimenter_header * ae =
        (struct ofp_action_experimenter_header*) buffer;
    Action::unpack(buffer);
    this->experimenter_ = ntoh32(ae->experimenter);
    return 0;
}

} //End of namespace of15

Action * Action::make_of15_action(uint16_t type) {
    switch (type) {
        case (of15::OFPAT_OUTPUT): {
            return new of15::OutputAction();
        }
        case (of15::OFPAT_COPY_TTL_OUT): {
            return new of15::CopyTTLOutAction();
        }
        case (of15::OFPAT_COPY_TTL_IN): {
            return new of15::CopyTTLInAction();
        }
        case (of15::OFPAT_SET_MPLS_TTL): {
            return new of15::SetMPLSTTLAction();
        }
        case (of15::OFPAT_DEC_MPLS_TTL): {
            return new of15::DecMPLSTTLAction();
        }
        case (of15::OFPAT_PUSH_VLAN): {
            return new of15::PushVLANAction();
        }
        case (of15::OFPAT_POP_VLAN): {
            return new of15::PopVLANAction();
        }
        case (of15::OFPAT_PUSH_MPLS): {
            return new of15::PushMPLSAction();
        }
        case (of15::OFPAT_POP_MPLS): {
            return new of15::PopMPLSAction();
        }
        case (of15::OFPAT_SET_QUEUE): {
            return new of15::SetQueueAction();
        }
        case (of15::OFPAT_GROUP): {
            return new of15::GroupAction();
        }
        case (of15::OFPAT_SET_NW_TTL): {
            return new of15::SetNWTTLAction();
        }
        case (of15::OFPAT_DEC_NW_TTL): {
            return new of15::DecNWTTLAction();
        }
        case (of15::OFPAT_SET_FIELD): {
            return new of15::SetFieldAction();
        }
        case (of15::OFPAT_PUSH_PBB): {
            return new of15::PushPBBAction();
        }
        case (of15::OFPAT_POP_PBB): {
            return new of15::PopPBBAction();
        }
        case (of15::OFPAT_COPY_FIELD): {
            return new of15::CopyFieldAction();
        }
        case (of15::OFPAT_EXPERIMENTER): {
            return new of15::ExperimenterAction();
        }
    }
    return NULL;
}

} //End of namespace fluid_msg

