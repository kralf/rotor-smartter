axt_message = '''
  struct axt_message {
    uint32_t num_points;
    uint32_t version;
    uint32_t scanner_type;
    uint32_t ecu_id;
    uint32_t timestamp_sensor;
    double start_angle;
    double end_angle;
    uint32_t scan_counter;

    uint8_t * channel;
    uint8_t * point_status;
    
    float *x;
    float *y;
    float *z;
    
    double timestamp;
  };
'''

elrob_point_6d_t = '''
  struct elrob_point_6d_t {
    double x;
    double y;
    double z;
    double phi;
    double theta;
    double psi;
  };
'''

elrob_smart_pos_message = '''
  struct elrob_smart_pos_message {
    elrob_point_6d_t pose;
    double velocity;
    double steering_angle;
    double timestamp;
    char * host;
  };
'''

elrob_smart_steering_message = '''
  struct elrob_smart_steering_message {
    double velocity;
    double steeringangle;
    double timestamp;
    char* host;
  };
'''

next_waypoint_message = '''
  struct next_waypoint_message {
    double x;
    double y;
    double timestamp;
    char* host;
  };
'''

state_machine_message = '''
  struct state_machine_message {
    int32_t   num_state_machine;
    int32_t * state_machine_id;
    int32_t * state_machine_state;
    double    timestamp;
    char *    host;
  };
'''

carmen_point_t = '''
  struct carmen_point_t {
    double x;
    double y;
    double theta;
  };
'''

carmen_localize_globalpos_message = '''
  struct carmen_localize_globalpos_message {
    carmen_point_t globalpos;
    carmen_point_t globalpos_std;
    carmen_point_t odometrypos;
    double globalpos_xy_cov;
    int32_t converged;
    double timestamp;
    char *host;
  };
'''

path_message = '''
  struct path_message {
    uint32_t point_count;
    double * x;
    double * y;
    double * theta;
    double velocity;
    double timestamp;
    char *host;
  };
'''

smart_status_message = '''
  struct smart_status_message {
    double gas_pos;      
    int32_t gear;            
    double steering_angle;

    double tv;            

    double rv_front_right;
    double rv_front_left; 
    double rv_rear_right; 
    double rv_rear_left;  

    double timestamp;
    char* host;
  };
'''