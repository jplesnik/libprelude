
/* Auto-generated by the GenerateIDMEFMessageIdH package */

#ifndef _LIBPRELUDE_IDMEF_MESSAGE_ID_H
#define _LIBPRELUDE_IDMEF_MESSAGE_ID_H

/*
 * Misc value
 */

#define IDMEF_MSG_OWN_FORMAT                                 253
#define IDMEF_MSG_END_OF_TAG                                 254

/*
 * Tag value
 */

#define IDMEF_MSG_ADDITIONAL_DATA_TAG 0
#define IDMEF_MSG_REFERENCE_TAG 1
#define IDMEF_MSG_CLASSIFICATION_TAG 2
#define IDMEF_MSG_USER_ID_TAG 3
#define IDMEF_MSG_USER_TAG 4
#define IDMEF_MSG_ADDRESS_TAG 5
#define IDMEF_MSG_PROCESS_TAG 6
#define IDMEF_MSG_WEB_SERVICE_TAG 7
#define IDMEF_MSG_SNMP_SERVICE_TAG 8
#define IDMEF_MSG_SERVICE_TAG 9
#define IDMEF_MSG_NODE_TAG 10
#define IDMEF_MSG_SOURCE_TAG 11
#define IDMEF_MSG_FILE_ACCESS_TAG 12
#define IDMEF_MSG_INODE_TAG 13
#define IDMEF_MSG_CHECKSUM_TAG 14
#define IDMEF_MSG_FILE_TAG 15
#define IDMEF_MSG_LINKAGE_TAG 16
#define IDMEF_MSG_TARGET_TAG 17
#define IDMEF_MSG_ANALYZER_TAG 18
#define IDMEF_MSG_ALERTIDENT_TAG 19
#define IDMEF_MSG_IMPACT_TAG 20
#define IDMEF_MSG_ACTION_TAG 21
#define IDMEF_MSG_CONFIDENCE_TAG 22
#define IDMEF_MSG_ASSESSMENT_TAG 23
#define IDMEF_MSG_TOOL_ALERT_TAG 24
#define IDMEF_MSG_CORRELATION_ALERT_TAG 25
#define IDMEF_MSG_OVERFLOW_ALERT_TAG 26
#define IDMEF_MSG_ALERT_TAG 27
#define IDMEF_MSG_HEARTBEAT_TAG 28


/*
 * IDMEF Additional_data
 */
#define IDMEF_MSG_ADDITIONAL_DATA_TYPE 29
#define IDMEF_MSG_ADDITIONAL_DATA_MEANING 30
#define IDMEF_MSG_ADDITIONAL_DATA_DATA 31


/*
 * IDMEF Reference
 */
#define IDMEF_MSG_REFERENCE_ORIGIN 29
#define IDMEF_MSG_REFERENCE_NAME 30
#define IDMEF_MSG_REFERENCE_URL 31
#define IDMEF_MSG_REFERENCE_MEANING 32


/*
 * IDMEF Classification
 */
#define IDMEF_MSG_CLASSIFICATION_IDENT 29
#define IDMEF_MSG_CLASSIFICATION_TEXT 30


/*
 * IDMEF User_id
 */
#define IDMEF_MSG_USER_ID_IDENT 29
#define IDMEF_MSG_USER_ID_TYPE 30
#define IDMEF_MSG_USER_ID_TTY 31
#define IDMEF_MSG_USER_ID_NAME 32
#define IDMEF_MSG_USER_ID_NUMBER 33


/*
 * IDMEF User
 */
#define IDMEF_MSG_USER_IDENT 29
#define IDMEF_MSG_USER_CATEGORY 30


/*
 * IDMEF Address
 */
#define IDMEF_MSG_ADDRESS_IDENT 29
#define IDMEF_MSG_ADDRESS_CATEGORY 30
#define IDMEF_MSG_ADDRESS_VLAN_NAME 31
#define IDMEF_MSG_ADDRESS_VLAN_NUM 32
#define IDMEF_MSG_ADDRESS_ADDRESS 33
#define IDMEF_MSG_ADDRESS_NETMASK 34


/*
 * IDMEF Process
 */
#define IDMEF_MSG_PROCESS_IDENT 29
#define IDMEF_MSG_PROCESS_NAME 30
#define IDMEF_MSG_PROCESS_PID 31
#define IDMEF_MSG_PROCESS_PATH 32
#define IDMEF_MSG_PROCESS_ARG 33
#define IDMEF_MSG_PROCESS_ENV 34


/*
 * IDMEF Web_service
 */
#define IDMEF_MSG_WEB_SERVICE_URL 29
#define IDMEF_MSG_WEB_SERVICE_CGI 30
#define IDMEF_MSG_WEB_SERVICE_HTTP_METHOD 31
#define IDMEF_MSG_WEB_SERVICE_ARG 32


/*
 * IDMEF Snmp_service
 */
#define IDMEF_MSG_SNMP_SERVICE_OID 29
#define IDMEF_MSG_SNMP_SERVICE_MESSAGE_PROCESSING_MODEL 30
#define IDMEF_MSG_SNMP_SERVICE_SECURITY_MODEL 31
#define IDMEF_MSG_SNMP_SERVICE_SECURITY_NAME 32
#define IDMEF_MSG_SNMP_SERVICE_SECURITY_LEVEL 33
#define IDMEF_MSG_SNMP_SERVICE_CONTEXT_NAME 34
#define IDMEF_MSG_SNMP_SERVICE_CONTEXT_ENGINE_ID 35
#define IDMEF_MSG_SNMP_SERVICE_COMMAND 36


/*
 * IDMEF Service
 */
#define IDMEF_MSG_SERVICE_IDENT 29
#define IDMEF_MSG_SERVICE_IP_VERSION 30
#define IDMEF_MSG_SERVICE_IANA_PROTOCOL_NUMBER 31
#define IDMEF_MSG_SERVICE_IANA_PROTOCOL_NAME 32
#define IDMEF_MSG_SERVICE_NAME 33
#define IDMEF_MSG_SERVICE_PORT 34
#define IDMEF_MSG_SERVICE_PORTLIST 35
#define IDMEF_MSG_SERVICE_PROTOCOL 36


/*
 * IDMEF Node
 */
#define IDMEF_MSG_NODE_IDENT 29
#define IDMEF_MSG_NODE_CATEGORY 30
#define IDMEF_MSG_NODE_LOCATION 31
#define IDMEF_MSG_NODE_NAME 32


/*
 * IDMEF Source
 */
#define IDMEF_MSG_SOURCE_IDENT 29
#define IDMEF_MSG_SOURCE_SPOOFED 30
#define IDMEF_MSG_SOURCE_INTERFACE 31


/*
 * IDMEF File_access
 */
#define IDMEF_MSG_FILE_ACCESS_PERMISSION 29


/*
 * IDMEF Inode
 */
#define IDMEF_MSG_INODE_CHANGE_TIME 29
#define IDMEF_MSG_INODE_NUMBER 30
#define IDMEF_MSG_INODE_MAJOR_DEVICE 31
#define IDMEF_MSG_INODE_MINOR_DEVICE 32
#define IDMEF_MSG_INODE_C_MAJOR_DEVICE 33
#define IDMEF_MSG_INODE_C_MINOR_DEVICE 34


/*
 * IDMEF Checksum
 */
#define IDMEF_MSG_CHECKSUM_VALUE 29
#define IDMEF_MSG_CHECKSUM_KEY 30
#define IDMEF_MSG_CHECKSUM_ALGORITHM 31


/*
 * IDMEF File
 */
#define IDMEF_MSG_FILE_IDENT 29
#define IDMEF_MSG_FILE_NAME 30
#define IDMEF_MSG_FILE_PATH 31
#define IDMEF_MSG_FILE_CREATE_TIME 32
#define IDMEF_MSG_FILE_MODIFY_TIME 33
#define IDMEF_MSG_FILE_ACCESS_TIME 34
#define IDMEF_MSG_FILE_DATA_SIZE 35
#define IDMEF_MSG_FILE_DISK_SIZE 36
#define IDMEF_MSG_FILE_CATEGORY 37
#define IDMEF_MSG_FILE_FSTYPE 38
#define IDMEF_MSG_FILE_FILE_TYPE 39


/*
 * IDMEF Linkage
 */
#define IDMEF_MSG_LINKAGE_CATEGORY 29
#define IDMEF_MSG_LINKAGE_NAME 30
#define IDMEF_MSG_LINKAGE_PATH 31


/*
 * IDMEF Target
 */
#define IDMEF_MSG_TARGET_IDENT 29
#define IDMEF_MSG_TARGET_DECOY 30
#define IDMEF_MSG_TARGET_INTERFACE 31


/*
 * IDMEF Analyzer
 */
#define IDMEF_MSG_ANALYZER_ANALYZERID 29
#define IDMEF_MSG_ANALYZER_NAME 30
#define IDMEF_MSG_ANALYZER_MANUFACTURER 31
#define IDMEF_MSG_ANALYZER_MODEL 32
#define IDMEF_MSG_ANALYZER_VERSION 33
#define IDMEF_MSG_ANALYZER_CLASS 34
#define IDMEF_MSG_ANALYZER_OSTYPE 35
#define IDMEF_MSG_ANALYZER_OSVERSION 36


/*
 * IDMEF Alertident
 */
#define IDMEF_MSG_ALERTIDENT_ALERTIDENT 29
#define IDMEF_MSG_ALERTIDENT_ANALYZERID 30


/*
 * IDMEF Impact
 */
#define IDMEF_MSG_IMPACT_SEVERITY 29
#define IDMEF_MSG_IMPACT_COMPLETION 30
#define IDMEF_MSG_IMPACT_TYPE 31
#define IDMEF_MSG_IMPACT_DESCRIPTION 32


/*
 * IDMEF Action
 */
#define IDMEF_MSG_ACTION_CATEGORY 29
#define IDMEF_MSG_ACTION_DESCRIPTION 30


/*
 * IDMEF Confidence
 */
#define IDMEF_MSG_CONFIDENCE_RATING 29
#define IDMEF_MSG_CONFIDENCE_CONFIDENCE 30


/*
 * IDMEF Assessment
 */


/*
 * IDMEF Tool_alert
 */
#define IDMEF_MSG_TOOL_ALERT_NAME 29
#define IDMEF_MSG_TOOL_ALERT_COMMAND 30


/*
 * IDMEF Correlation_alert
 */
#define IDMEF_MSG_CORRELATION_ALERT_NAME 29


/*
 * IDMEF Overflow_alert
 */
#define IDMEF_MSG_OVERFLOW_ALERT_PROGRAM 29
#define IDMEF_MSG_OVERFLOW_ALERT_SIZE 30
#define IDMEF_MSG_OVERFLOW_ALERT_BUFFER 31


/*
 * IDMEF Alert
 */
#define IDMEF_MSG_ALERT_MESSAGEID 29
#define IDMEF_MSG_ALERT_CREATE_TIME 30
#define IDMEF_MSG_ALERT_DETECT_TIME 31
#define IDMEF_MSG_ALERT_ANALYZER_TIME 32


/*
 * IDMEF Heartbeat
 */
#define IDMEF_MSG_HEARTBEAT_MESSAGEID 29
#define IDMEF_MSG_HEARTBEAT_CREATE_TIME 30
#define IDMEF_MSG_HEARTBEAT_ANALYZER_TIME 31
#define IDMEF_MSG_HEARTBEAT_HEARTBEAT_INTERVAL 32


/*
 * IDMEF Message
 */
#define IDMEF_MSG_MESSAGE_VERSION 29


#endif /* _LIBPRELUDE_IDMEF_MESSAGE_ID_H */
