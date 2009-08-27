#ifndef SMART_ROTOR_INTERFACES_H
#define SMART_ROTOR_INTERFACES_H


#include <rotor/Rotor.h>

ROTOR_DEFINE_TYPE( axt_message,
  struct axt_message {
    uint32_t num_points;
    uint32_t version;
    uint32_t scanner_type;
    uint32_t ecu_id;
    uint32_t timestamp_sensor;
    double start_angle;
    double end_angle;
    uint32_t scan_counter;

    int8_t * channel;
    int8_t * point_status;
    
    float *x;
    float *y;
    float *z;

    double timestamp;
  };
)


#endif SMART_ROTOR_INTERFACES_H