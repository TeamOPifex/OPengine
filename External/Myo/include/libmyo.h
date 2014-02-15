// Copyright (C) 2013-2014 Thalmic Labs Inc.
// Confidential and not for redistribution. See LICENSE.txt.
#ifndef LIBMYO_H
#define LIBMYO_H

#include <stdint.h>

#include "libmyo/detail/visibility.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void* libmyo_hub_t;

/// \defgroup errors Error Handling
/// @{

/// Function result codes.
/// All libmyo functions that can fail return a value of this type.
typedef enum {
    libmyo_success,
    libmyo_error,
    libmyo_error_invalid_argument
} libmyo_result_t;

/// Opaque handle to detailed error information.
typedef void* libmyo_error_details_t;

/// Return a null-terminated string with a detailed error message.
LIBMYO_EXPORT
const char* libmyo_error_cstring(libmyo_error_details_t);

/// Returns the kind of error that occurred.
LIBMYO_EXPORT
libmyo_result_t libmyo_error_kind(libmyo_error_details_t);

/// Free the resources allocated by an error object.
LIBMYO_EXPORT
void libmyo_free_error_details(libmyo_error_details_t);

/// @}

/// Initialize a hub.
/// Can be called concurrently with other calls to libmyo_init_hub() and libmyo_shutdown_hub().
/// Only one hub can exist at a time.
/// @returns libmyo_success if init is successful, otherwise:
///  - libmyo_error_invalid_argument if \a out_hub is NULL
///  - libmyo_error_invalid_argument results from having two hubs if a hub currently exists
LIBMYO_EXPORT
libmyo_result_t libmyo_init_hub(libmyo_hub_t* out_hub, libmyo_error_details_t* out_error);

/// Free the resources allocated to a hub.
/// Can be called concurrently with other calls to libmyo_init_hub() and libmyo_shutdown_hub().
/// @returns libmyo_success if shutdown is successful, otherwise:
///  - libmyo_error_invalid_argument if \a hub is NULL
///  - libmyo_error if \a hub is not a valid \a hub
LIBMYO_EXPORT
libmyo_result_t libmyo_shutdown_hub(libmyo_hub_t hub, libmyo_error_details_t* out_error);

/// Opaque type corresponding to a known Myo device.
typedef void* libmyo_myo_t;

/// Retrieve the MAC address of a Myo.
/// The MAC address is unique to the physical Myo, and is a 48-bit number.
LIBMYO_EXPORT
uint64_t libmyo_get_mac_address(libmyo_myo_t myo);

/// Types of vibration
typedef enum {
    libmyo_vibration_short,
    libmyo_vibration_medium,
    libmyo_vibration_long
} libmyo_vibration_type_t;

/// Vibrate the given myo
/// Can be called when a Myo is paired
/// @returns libmyo_success if the Myo successfully vibrated, otherwise
///  - libmyo_error_invalid_argument if \a myo is NULL
LIBMYO_EXPORT
libmyo_result_t libmyo_vibrate(libmyo_myo_t myo, libmyo_vibration_type_t type, libmyo_error_details_t* out_error);

/// @defgroup libmyo_pairing Pairing
/// Pairing with a Myo means we have discovered a nearby Myo device and will attempt to maintain a connection with it.
/// Pairing can be initiated with a function like libmyo_pair_any().
/// In order to actually pair with a device, libmyo_run needs to be called until a libmyo_event_paired event occurs.
/// Only paired devices will generate events.
/// @{

/// Initiate pairing with \a count Myos.
/// Currently only one Myo is supported.
///
/// @returns libmyo_success if pairing initiated successfully, otherwise
///  - libmyo_error_invalid_argument if \a hub is NULL
///  - libmyo_error if count is 0
///  - libmyo_error if count is greater than 1
///  - libmyo_error if pairing has already been initiated
LIBMYO_EXPORT
libmyo_result_t libmyo_pair_any(libmyo_hub_t hub, unsigned int count, libmyo_error_details_t* out_error);

/// Initiate pairing with the device with MAC address \a mac_address.
/// @returns libmyo_success if pairing initiated successfully, otherwise
///  - libmyo_error if mac_address is invalid (e.g. is zero or takes up more than the low 48 bits)
///  - libmyo_error if pairing has already been initiated
///  - libmyo_error_invalid_argument if \a hub is NULL
LIBMYO_EXPORT
libmyo_result_t libmyo_pair_by_mac_address(libmyo_hub_t hub, uint64_t mac_address, libmyo_error_details_t* out_errors);

/// @}

/// @defgroup libmyo_poses Pose recognition.
/// @{

/// Supported poses.
typedef enum {
    libmyo_pose_none,           ///< Rest pose.
    libmyo_pose_fist,           ///< User is making a fist.
    libmyo_pose_wave_in,        ///< User has an open palm rotated towards the posterior of their wrist.
    libmyo_pose_wave_out,       ///< User has an open palm rotated towards the anterior of their wrist.
    libmyo_pose_fingers_spread, ///< User has an open palm with their fingers spread away from each other.

    libmyo_num_poses,           ///< Number of poses supported; not a valid pose.
} libmyo_pose_t;

/// @}

/// @defgroup libmyo_training Training.
/// @{

/// Return 1 if a training profile is available, 0 otherwise.
LIBMYO_EXPORT
int libmyo_training_is_available(libmyo_myo_t myo);

/// Data to perform a training run.
typedef void* libmyo_training_dataset_t;

/// Allocate a training data object.
/// @returns
///  - libmyo_error_invalid_argument if \a myo is NULL.
///  - libmyo_error_invalid_argument if \a out_dataset is NULL.
LIBMYO_EXPORT
libmyo_result_t libmyo_training_create_dataset(libmyo_myo_t myo, libmyo_training_dataset_t* out_dataset,
                                               libmyo_error_details_t* out_error);

/// A callback that's called while libmyo_training_collect_data() is executing.
/// value is set to a value between 0 and 255 in order that can be visualized to provide feedback to the user as
/// training data is collected, corresponding in some way to the electrical signal measured by the Myo.
/// progress is set to a value between 0 and 255 which represents the overall progress of the data collection.
typedef void (*libmyo_training_collect_status_t)(void* user_data, uint8_t value, uint8_t progress);

/// Collect training data for the given pose into the provided data object.
/// Must not be called concurrently with libmyo_run().
/// Blocks until enough data has been collected.
/// If \a callback is not NULL, call it periodically with updates on the collection progress.
/// @returns libmyo_success if data was collected successfully, otherwise
///  - libmyo_error_invalid_argument if \a dataset is NULL.
///  - libmyo_error_invalid_argument if \a pose is not a valid \a pose
LIBMYO_EXPORT
libmyo_result_t libmyo_training_collect_data(libmyo_training_dataset_t dataset, libmyo_pose_t pose,
                                             libmyo_training_collect_status_t callback, void* user_data,
                                             libmyo_error_details_t* out_error);

/// Train pose recognition from collected training data.
/// @returns libmyo_success if training succeeded, otherwise
///  - libmyo_error if one of the poses is missing data
///  - libmyo_error_invalid_argument if \a dataset is NULL
LIBMYO_EXPORT
libmyo_result_t libmyo_training_train_from_dataset(libmyo_training_dataset_t dataset, libmyo_error_details_t* out_error);

/// Release any resources associated with the provided training data.
LIBMYO_EXPORT
void libmyo_training_free_dataset(libmyo_training_dataset_t dataset);

/// Load training results from the given filename.
/// If filename is NULL, load the profile from the default profile file.
/// @returns
///  - libmyo_error if the file could not be read
///  - libmyo_error_invalid_argument if \a myo is NULL.
LIBMYO_EXPORT
libmyo_result_t libmyo_training_load_profile(libmyo_myo_t myo, const char* filename,
                                             libmyo_error_details_t* out_error);

/// Store the current training profile using the provided filename.
/// If filename is NULL, store the profile in the default profile file.
/// @returns libmyo_success if the profile was stored successfully, otherwise
///  - libmyo_error if there is no current training profile
///  - libmyo_error if the file could not be written
///  - libmyo_error_invalid_argument if \a myo is NULL.
LIBMYO_EXPORT
libmyo_result_t libmyo_training_store_profile(libmyo_myo_t myo, const char* filename,
                                              libmyo_error_details_t* out_error);

/// Asynchronously send the given training data to Thalmic Labs.
/// @returns libmyo_success if the send was initiated, otherwise
///  - libmyo_error_invalid_argument if \a dataset is NULL
LIBMYO_EXPORT
libmyo_result_t libmyo_training_send_training_data(libmyo_training_dataset_t dataset,
                                                   libmyo_error_details_t* out_error);

/// Attach a name and value to the given training data. These annotations will be included
/// if training data is sent to Thalmic Labs.
/// \note Using the same name in a subsequent call results in an update.
/// @returns libmyo_success if the annotation was added successfully, otherwise
///  - libmyo_error_invalid_argument if \a dataset is NULL
///  - libmyo_error_invalid_argument if \a name or \a value is NULL
///  - libmyo_error_invalid_argument if \a name is longer than 80 characters.
///  - libmyo_error_invalid_argument if \a value is longer than 255 characters.
LIBMYO_EXPORT
libmyo_result_t libmyo_training_annotate_training_data(libmyo_training_dataset_t dataset,
                                                       const char* name,
                                                       const char* value,
                                                       libmyo_error_details_t* out_error);
/// @}

/// @defgroup libmyo_events Event Handling
/// @{

/// Types of events.
typedef enum {
    libmyo_event_paired, ///< Successfully paired with a Myo.
    libmyo_event_connected, ///< A Myo has successfully connected.
    libmyo_event_disconnected, ///< A Myo has been disconnected.
    libmyo_event_orientation, ///< Orientation data has been received.
    libmyo_event_pose, ///< A change in pose has been detected. @see libmyo_pose_t.
} libmyo_event_type_t;

/// Information about an event.
typedef void* libmyo_event_t;

/// Retrieve the type of an event.
LIBMYO_EXPORT
libmyo_event_type_t libmyo_event_get_type(libmyo_event_t event);

/// Retrieve the timestamp of an event.
/// @see libmyo_now() for details on timestamps.
LIBMYO_EXPORT
uint64_t libmyo_event_get_timestamp(libmyo_event_t event);

/// Retrieve the current timestamp.
/// Timestamps are 64 bit unsigned integers that correspond to a number of microseconds since some (unspecified)
/// period in time. Timestamps are monotonically non-decreasing. You can use libmyo_now() to, for example, ignore
/// events that occur before or after a particular moment in time.
/// @see libmyo_event_get_timestamp()
LIBMYO_EXPORT
uint64_t libmyo_now();

/// Retrieve the Myo associated with an event.
LIBMYO_EXPORT
libmyo_myo_t libmyo_event_get_myo(libmyo_event_t event);

/// Index into orientation data, which is provided as a quaternion.
/// Orientation data is returned as a quaternion of 16 bit integers, represented as `w + x * i + y * j + z * k`.
typedef enum {
    libmyo_orientation_x = 0, ///< First component of the quaternion's vector part
    libmyo_orientation_y = 1, ///< Second component of the quaternion's vector part
    libmyo_orientation_z = 2, ///< Third component of the quaternion's vector part
    libmyo_orientation_w = 3, ///< Scalar component of the quaternion.
} libmyo_orientation_index;

/// Retrieve orientation data associated with an event.
/// Valid for libmyo_event_orientation events only.
/// @see libmyo_orientation_index
LIBMYO_EXPORT
int16_t libmyo_event_get_orientation(libmyo_event_t event, libmyo_orientation_index index);

/// Retrieve raw accelerometer data associated with an event in units of g.
/// Valid for libmyo_event_orientation events only.
/// Requires `index < 3`.
LIBMYO_EXPORT
float libmyo_event_get_accelerometer(libmyo_event_t event, unsigned int index);

/// Retrieve raw gyroscope data associated with an event in units of deg/s.
/// Valid for libmyo_event_orientation events only.
/// Requires `index < 3`.
LIBMYO_EXPORT
float libmyo_event_get_gyroscope(libmyo_event_t event, unsigned int index);

/// Retrieve the pose associated with an event.
/// Valid for libmyo_event_pose events only.
LIBMYO_EXPORT
libmyo_pose_t libmyo_event_get_pose(libmyo_event_t event);

/// Return type for event handlers.
typedef enum {
    libmyo_handler_continue, ///< Continue processing events
    libmyo_handler_stop,     ///< Stop processing events
} libmyo_handler_result_t;

/// Callback function type to handle events as they occur from libmyo_run().
typedef libmyo_handler_result_t (*libmyo_handler_t)(void* user_data, libmyo_event_t event);

/// Process events and call the provided callback as they occur.
/// Runs for up to approximately \a duration_ms milliseconds or until a called handler returns libmyo_handler_stop.
/// @returns libmyo_success after a successful run, otherwise
///  - libmyo_error_invalid_argument if \a hub is NULL
///  - libmyo_error_invalid_argument if \a handler is NULL
LIBMYO_EXPORT
libmyo_result_t libmyo_run(libmyo_hub_t hub, unsigned int duration_ms, libmyo_handler_t handler, void* user_data, libmyo_error_details_t* out_error);

/// @}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBMYO_H
