
/* Auto-generated by the GenerateIDMEFMessageWriteH package (Tue Jul 20 14:53:42 2004) */

#ifndef _LIBPRELUDE_IDMEF_MESSAGE_WRITE_H
#define _LIBPRELUDE_IDMEF_MESSAGE_WRITE_H

#include "prelude-message-buffered.h"


void idmef_write_time(prelude_msgbuf_t *msg, uint8_t tag, idmef_time_t *data);
void idmef_write_additional_data(prelude_msgbuf_t *msg, idmef_additional_data_t *additional_data);
void idmef_write_classification(prelude_msgbuf_t *msg, idmef_classification_t *classification);
void idmef_write_userid(prelude_msgbuf_t *msg, idmef_userid_t *userid);
void idmef_write_user(prelude_msgbuf_t *msg, idmef_user_t *user);
void idmef_write_address(prelude_msgbuf_t *msg, idmef_address_t *address);
void idmef_write_process(prelude_msgbuf_t *msg, idmef_process_t *process);
void idmef_write_webservice(prelude_msgbuf_t *msg, idmef_webservice_t *webservice);
void idmef_write_snmpservice(prelude_msgbuf_t *msg, idmef_snmpservice_t *snmpservice);
void idmef_write_service(prelude_msgbuf_t *msg, idmef_service_t *service);
void idmef_write_node(prelude_msgbuf_t *msg, idmef_node_t *node);
void idmef_write_source(prelude_msgbuf_t *msg, idmef_source_t *source);
void idmef_write_file_access(prelude_msgbuf_t *msg, idmef_file_access_t *file_access);
void idmef_write_inode(prelude_msgbuf_t *msg, idmef_inode_t *inode);
void idmef_write_file(prelude_msgbuf_t *msg, idmef_file_t *file);
void idmef_write_linkage(prelude_msgbuf_t *msg, idmef_linkage_t *linkage);
void idmef_write_target(prelude_msgbuf_t *msg, idmef_target_t *target);
void idmef_write_analyzer(prelude_msgbuf_t *msg, idmef_analyzer_t *analyzer);
void idmef_write_alertident(prelude_msgbuf_t *msg, idmef_alertident_t *alertident);
void idmef_write_impact(prelude_msgbuf_t *msg, idmef_impact_t *impact);
void idmef_write_action(prelude_msgbuf_t *msg, idmef_action_t *action);
void idmef_write_confidence(prelude_msgbuf_t *msg, idmef_confidence_t *confidence);
void idmef_write_assessment(prelude_msgbuf_t *msg, idmef_assessment_t *assessment);
void idmef_write_tool_alert(prelude_msgbuf_t *msg, idmef_tool_alert_t *tool_alert);
void idmef_write_correlation_alert(prelude_msgbuf_t *msg, idmef_correlation_alert_t *correlation_alert);
void idmef_write_overflow_alert(prelude_msgbuf_t *msg, idmef_overflow_alert_t *overflow_alert);
void idmef_write_alert(prelude_msgbuf_t *msg, idmef_alert_t *alert);
void idmef_write_heartbeat(prelude_msgbuf_t *msg, idmef_heartbeat_t *heartbeat);
void idmef_write_message(prelude_msgbuf_t *msg, idmef_message_t *message);

#endif /* _LIBPRELUDE_IDMEF_MESSAGE_WRITE_H */
