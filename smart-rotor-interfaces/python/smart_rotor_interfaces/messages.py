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

    int8_t * channel;
    int8_t * point_status;
    
    float *x;
    float *y;
    float *z;

    double timestamp;
  };
'''

carmen_point_t = '''
  struct carmen_point_t {
    double x;
    double y;
    double theta;
  };
'''

carmen_base_odometry_message = '''
  struct carmen_base_odometry_message {
    double x;
    double y;
    double theta;
    double tv;
    double rv;
    double acceleration;
    double timestamp;
    char *host;
  };
'''

carmen_base_velocity_message = '''
  struct carmen_base_velocity_message {
    double tv;
    double rv;
    double timestamp;
    char *host;
  };
'''

carmen_laser_laser_config_t = '''
  struct carmen_laser_laser_config_t {
    int32_t  laser_type;
    double start_angle;                   
    double fov;
    double angular_resolution;
    double maximum_range;
    double accuracy;
    int32_t remission_mode;
  };
'''

carmen_laser_laser_message = '''
  struct carmen_laser_laser_message {
    int32_t id;
    carmen_laser_laser_config_t config;
    int32_t num_readings;
    float *range;
    int32_t num_remissions;
    float *remission;
    double timestamp;
    char *host;
  };
'''

carmen_robot_laser_message = '''
  struct carmen_robot_laser_message {
    int32_t id;
    carmen_laser_laser_config_t config;
    int32_t num_readings;                  
    float * range;                      
    int8_t * tooclose;                    
    int32_t num_remissions;                
    float * remission;                  
    carmen_point_t laser_pose;         
    carmen_point_t robot_pose;         
    double tv;
    double rv;                     
    double forward_safety_dist;
    double side_safety_dist;
    double turn_axis;
    double timestamp;                    
    char *host;                          
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

smart_velocity_message = '''
  struct smart_velocity_message {
    double tv;
    double steering_angle; 
    double timestamp;
    char* host;
  };
'''

smart_status_message = '''
  struct smart_status_message {
    double gas_pos;
    int gear;      
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
