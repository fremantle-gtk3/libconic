/*
  libconic - Internet Connectivity library
  
  Copyright (C) 2006 Nokia Corporation. All rights reserved.

  Contact: Patrik Flykt <patrik.flykt@nokia.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public License
  version 2.1 as published by the Free Software Foundation.

  This library is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
  02110-1301 USA
*/

#ifndef CONICSTATISTICSEVENT_PRIVATE_H
#define CONICSTATISTICSEVENT_PRIVATE_H

G_BEGIN_DECLS

ConIcStatisticsEvent *con_ic_statistics_event_new(void);

void con_ic_statistics_event_set_time_active(ConIcStatisticsEvent *statistics,
					     guint time);
void con_ic_statistics_event_set_signal_strength(ConIcStatisticsEvent *statistics,
						 guint strength);
void con_ic_statistics_event_set_rx_packets(ConIcStatisticsEvent *statistics,
					    guint64 packets);
void con_ic_statistics_event_set_tx_packets(ConIcStatisticsEvent *statistics,
					    guint64 packets);
void con_ic_statistics_event_set_rx_bytes(ConIcStatisticsEvent *statistics,
					  guint64 bytes);
void con_ic_statistics_event_set_tx_bytes(ConIcStatisticsEvent *statistics,
					  guint64 bytes);


G_END_DECLS

#endif /* CONICSTATISTICSEVENT_PRIVATE_H */
