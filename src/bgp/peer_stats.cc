/*
 * Copyright (c) 2016 Juniper Networks, Inc. All rights reserved.
 */

#include "bgp/peer_stats.h"

void PeerStats::FillProtoStats(const IPeerDebugStats::ProtoStats &stats,
                               PeerProtoStats *proto_stats) {
    proto_stats->set_open(stats.open);
    proto_stats->set_keepalive(stats.keepalive);
    proto_stats->set_close(stats.close);
    proto_stats->set_update(stats.update);
    proto_stats->set_notification(stats.notification);
    proto_stats->set_total(stats.open + stats.keepalive + stats.close +
        stats.update + stats.notification);
}

void PeerStats::FillRouteUpdateStats(const IPeerDebugStats::UpdateStats &stats,
                                     PeerUpdateStats *rt_stats) {
    rt_stats->set_total(stats.total);
    rt_stats->set_reach(stats.reach);
    rt_stats->set_unreach(stats.unreach);
}

void PeerStats::FillRxErrorStats(const IPeerDebugStats::RxErrorStats &src,
                                 PeerRxErrorStats *dest) {
    PeerRxInet6ErrorStats error_stats;
    error_stats.set_bad_inet6_xml_token_count(src.inet6_bad_xml_token_count);
    error_stats.set_bad_inet6_prefix_count(src.inet6_bad_prefix_count);
    error_stats.set_bad_inet6_nexthop_count(src.inet6_bad_nexthop_count);
    error_stats.set_bad_inet6_afi_safi_count(src.inet6_bad_afi_safi_count);
    dest->set_inet6_error_stats(error_stats);
}

void PeerStats::FillRxRouteStats(const IPeerDebugStats::RxRouteStats &src,
                                 PeerRxRouteStats *dest) {
    dest->set_total_path_count(src.total_path_count);
    dest->set_primary_path_count(src.primary_path_count);
}

void PeerStats::FillPeerUpdateStats(const IPeerDebugStats *peer_stats,
                                    PeerUpdateStats *rt_stats_rx,
                                    PeerUpdateStats *rt_stats_tx) {

    IPeerDebugStats::UpdateStats update_stats_rx;
    peer_stats->GetRxRouteUpdateStats(&update_stats_rx);
    FillRouteUpdateStats(update_stats_rx, rt_stats_rx);

    IPeerDebugStats::UpdateStats update_stats_tx;
    peer_stats->GetTxRouteUpdateStats(&update_stats_tx);
    FillRouteUpdateStats(update_stats_tx, rt_stats_tx);
}

void PeerStats::FillPeerUpdateStats(const IPeerDebugStats *peer_stats,
                                    PeerStatsData *peer_stats_data) {
    PeerUpdateStats stats_rx;
    PeerUpdateStats stats_tx;

    FillPeerUpdateStats(peer_stats, &stats_rx, &stats_tx);
    peer_stats_data->set_raw_rx_update_stats(stats_rx);
    peer_stats_data->set_raw_tx_update_stats(stats_tx);
}

void PeerStats::FillPeerDebugStats(const IPeerDebugStats *peer_stats,
                                   PeerStatsInfo *stats) {
    PeerProtoStats proto_stats_tx;
    PeerProtoStats proto_stats_rx;
    PeerUpdateStats rt_stats_rx;
    PeerUpdateStats rt_stats_tx;
    PeerRxErrorStats dest_error_stats_rx;
    PeerRxRouteStats dest_route_stats_rx;

    IPeerDebugStats::ProtoStats stats_rx;
    peer_stats->GetRxProtoStats(&stats_rx);
    FillProtoStats(stats_rx, &proto_stats_rx);

    IPeerDebugStats::ProtoStats stats_tx;
    peer_stats->GetTxProtoStats(&stats_tx);
    FillProtoStats(stats_tx, &proto_stats_tx);

    FillPeerUpdateStats(peer_stats, &rt_stats_rx, &rt_stats_tx);

    IPeerDebugStats::RxErrorStats src_error_stats_rx;
    peer_stats->GetRxErrorStats(&src_error_stats_rx);
    FillRxErrorStats(src_error_stats_rx, &dest_error_stats_rx);

    IPeerDebugStats::RxRouteStats src_route_stats_rx;
    peer_stats->GetRxRouteStats(&src_route_stats_rx);
    FillRxRouteStats(src_route_stats_rx, &dest_route_stats_rx);

    stats->set_rx_proto_stats(proto_stats_rx);
    stats->set_tx_proto_stats(proto_stats_tx);
    stats->set_rx_update_stats(rt_stats_rx);
    stats->set_tx_update_stats(rt_stats_tx);
    stats->set_rx_error_stats(dest_error_stats_rx);
    stats->set_rx_route_stats(dest_route_stats_rx);
}
