#include "../device.h"

#include <string.h>
#include <math.h>
#include <hidapi.h>
//#define DEBUG

static struct device device_g933;

static const uint16_t PRODUCT_ID = 0x0a5b;

#define NUMBER_OF_VMAP_LINES 597
// voltage map. [][0] is voltage [][1] is battery level for that voltage.
static const float voltage_map[NUMBER_OF_VMAP_LINES][2] = {{4057,100.00}, {4050,99.83}, {4043,99.68}, {4037,99.51}, {4034,99.34}, {4032,99.19}, {4027,99.02}, {4025,98.85}, {4022,98.70}, {4020,98.53}, {4018,98.35}, {4015,98.21}, {4012,98.03}, {4011,97.86}, {4010,97.69}, {4006,97.54}, {4004,97.37}, {4002,97.20}, {4001,97.03}, {3999,96.88}, {3997,96.71}, {3994,96.54}, {3993,96.39}, {3991,96.22}, {3990,96.04}, {3988,95.87}, {3985,95.72}, {3985,95.55}, {3982,95.38}, {3980,95.21}, {3978,95.06}, {3978,94.89}, {3975,94.72}, {3974,94.55}, {3972,94.40}, {3970,94.23}, {3967,94.05}, {3965,93.88}, {3965,93.73}, {3963,93.56}, {3962,93.39}, {3959,93.22}, {3958,93.07}, {3955,92.90}, {3955,92.73}, {3952,92.56}, {3952,92.41}, {3951,92.24}, {3948,92.06}, {3947,91.89}, {3945,91.72}, {3945,91.57}, {3942,91.40}, {3940,91.23}, {3940,91.06}, {3939,90.88}, {3938,90.74}, {3937,90.57}, {3935,90.39}, {3933,90.22}, {3933,90.07}, {3933,89.90}, {3930,89.73}, {3930,89.56}, {3928,89.39}, {3926,89.24}, {3925,89.07}, {3924,88.89}, {3924,88.72}, {3921,88.55}, {3920,88.40}, {3918,88.23}, {3918,88.06}, {3915,87.89}, {3915,87.71}, {3912,87.57}, {3911,87.40}, {3909,87.22}, {3907,87.05}, {3906,86.88}, {3904,86.73}, {3901,86.56}, {3898,86.39}, {3897,86.22}, {3895,86.04}, {3894,85.87}, {3891,85.72}, {3889,85.55}, {3887,85.38}, {3884,85.21}, {3883,85.04}, {3881,84.86}, {3879,84.72}, {3876,84.55}, {3875,84.37}, {3873,84.20}, {3868,84.03}, {3868,83.86}, {3866,83.69}, {3864,83.51}, {3863,83.37}, {3861,83.19}, {3858,83.02}, {3858,82.85}, {3856,82.68}, {3854,82.51}, {3851,82.33}, {3851,82.16}, {3851,81.99}, {3848,81.84}, {3847,81.67}, {3844,81.50}, {3845,81.33}, {3842,81.15}, {3841,80.98}, {3840,80.81}, {3839,80.64}, {3837,80.47}, {3836,80.29}, {3836,80.15}, {3833,79.98}, {3831,79.80}, {3831,79.63}, {3831,79.46}, {3830,79.29}, {3828,79.12}, {3827,78.94}, {3825,78.77}, {3824,78.60}, {3823,78.43}, {3823,78.26}, {3821,78.08}, {3820,77.91}, {3819,77.74}, {3817,77.57}, {3818,77.42}, {3815,77.25}, {3814,77.08}, {3814,76.90}, {3812,76.73}, {3810,76.56}, {3809,76.39}, {3809,76.22}, {3807,76.04}, {3809,75.87}, {3805,75.70}, {3805,75.53}, {3803,75.36}, {3802,75.18}, {3801,75.01}, {3800,74.84}, {3800,74.67}, {3797,74.50}, {3797,74.32}, {3796,74.15}, {3795,73.98}, {3794,73.81}, {3792,73.64}, {3791,73.46}, {3790,73.29}, {3789,73.12}, {3788,72.95}, {3788,72.78}, {3786,72.60}, {3784,72.43}, {3782,72.26}, {3782,72.06}, {3782,71.89}, {3780,71.72}, {3778,71.55}, {3776,71.38}, {3774,71.20}, {3776,71.03}, {3772,70.86}, {3772,70.69}, {3772,70.52}, {3771,70.34}, {3768,70.17}, {3767,70.00}, {3766,69.83}, {3764,69.66}, {3764,69.48}, {3762,69.31}, {3762,69.14}, {3761,68.97}, {3759,68.77}, {3758,68.60}, {3756,68.43}, {3755,68.26}, {3754,68.08}, {3754,67.91}, {3752,67.74}, {3751,67.57}, {3748,67.40}, {3748,67.22}, {3746,67.05}, {3746,66.88}, {3745,66.68}, {3742,66.51}, {3741,66.34}, {3741,66.17}, {3739,66.00}, {3738,65.82}, {3737,65.65}, {3736,65.48}, {3736,65.31}, {3734,65.14}, {3732,64.94}, {3733,64.77}, {3730,64.59}, {3729,64.42}, {3727,64.25}, {3726,64.08}, {3725,63.91}, {3726,63.73}, {3724,63.56}, {3722,63.37}, {3722,63.19}, {3719,63.02}, {3719,62.85}, {3718,62.68}, {3717,62.51}, {3717,62.33}, {3716,62.16}, {3714,61.97}, {3713,61.79}, {3713,61.62}, {3711,61.45}, {3709,61.28}, {3708,61.11}, {3709,60.93}, {3706,60.74}, {3706,60.57}, {3703,60.39}, {3704,60.22}, {3703,60.05}, {3701,59.88}, {3701,59.68}, {3701,59.51}, {3699,59.34}, {3698,59.16}, {3697,58.99}, {3697,58.82}, {3695,58.62}, {3695,58.45}, {3694,58.28}, {3693,58.11}, {3690,57.94}, {3690,57.74}, {3691,57.57}, {3689,57.40}, {3688,57.22}, {3687,57.05}, {3688,56.86}, {3687,56.68}, {3685,56.51}, {3685,56.34}, {3683,56.17}, {3683,55.97}, {3681,55.80}, {3682,55.63}, {3681,55.45}, {3680,55.28}, {3679,55.09}, {3678,54.91}, {3678,54.74}, {3677,54.57}, {3676,54.37}, {3675,54.20}, {3673,54.03}, {3675,53.86}, {3672,53.66}, {3672,53.49}, {3671,53.32}, {3671,53.14}, {3670,52.97}, {3669,52.78}, {3669,52.60}, {3667,52.43}, {3667,52.26}, {3666,52.06}, {3666,51.89}, {3665,51.72}, {3664,51.55}, {3664,51.35}, {3663,51.18}, {3661,51.01}, {3663,50.84}, {3662,50.64}, {3660,50.47}, {3660,50.29}, {3658,50.12}, {3658,49.93}, {3657,49.75}, {3657,49.58}, {3656,49.41}, {3657,49.21}, {3655,49.04}, {3656,48.87}, {3654,48.70}, {3654,48.50}, {3653,48.33}, {3653,48.16}, {3653,47.99}, {3651,47.79}, {3650,47.62}, {3650,47.44}, {3651,47.27}, {3650,47.08}, {3648,46.90}, {3648,46.73}, {3646,46.56}, {3646,46.36}, {3646,46.19}, {3646,46.02}, {3643,45.82}, {3645,45.65}, {3644,45.48}, {3643,45.31}, {3642,45.11}, {3641,44.94}, {3641,44.77}, {3640,44.57}, {3640,44.40}, {3640,44.23}, {3638,44.05}, {3639,43.86}, {3639,43.69}, {3638,43.51}, {3638,43.32}, {3637,43.14}, {3636,42.97}, {3636,42.80}, {3635,42.60}, {3636,42.43}, {3636,42.26}, {3634,42.06}, {3635,41.89}, {3632,41.72}, {3634,41.55}, {3634,41.35}, {3632,41.18}, {3632,41.01}, {3631,40.81}, {3632,40.64}, {3631,40.47}, {3631,40.27}, {3629,40.10}, {3629,39.93}, {3629,39.75}, {3628,39.56}, {3627,39.39}, {3626,39.21}, {3628,39.02}, {3626,38.85}, {3625,38.67}, {3624,38.48}, {3625,38.30}, {3625,38.13}, {3625,37.94}, {3624,37.76}, {3623,37.59}, {3622,37.40}, {3623,37.22}, {3624,37.05}, {3623,36.86}, {3622,36.68}, {3621,36.51}, {3620,36.34}, {3621,36.14}, {3621,35.97}, {3621,35.80}, {3620,35.60}, {3619,35.43}, {3619,35.26}, {3621,35.06}, {3617,34.89}, {3618,34.72}, {3618,34.52}, {3617,34.35}, {3617,34.18}, {3617,33.98}, {3616,33.81}, {3615,33.64}, {3615,33.44}, {3614,33.27}, {3616,33.10}, {3614,32.90}, {3614,32.73}, {3614,32.56}, {3613,32.36}, {3614,32.19}, {3612,32.01}, {3613,31.82}, {3612,31.65}, {3611,31.47}, {3610,31.28}, {3610,31.11}, {3610,30.93}, {3609,30.74}, {3609,30.57}, {3609,30.39}, {3609,30.20}, {3609,30.02}, {3609,29.85}, {3608,29.66}, {3606,29.48}, {3607,29.29}, {3606,29.12}, {3604,28.94}, {3605,28.75}, {3606,28.57}, {3605,28.40}, {3602,28.21}, {3603,28.03}, {3603,27.86}, {3602,27.67}, {3600,27.49}, {3602,27.32}, {3600,27.13}, {3600,26.95}, {3600,26.76}, {3600,26.58}, {3598,26.41}, {3598,26.22}, {3597,26.04}, {3598,25.87}, {3596,25.68}, {3597,25.50}, {3596,25.31}, {3596,25.14}, {3595,24.96}, {3593,24.77}, {3591,24.59}, {3592,24.40}, {3592,24.23}, {3593,24.05}, {3591,23.86}, {3592,23.69}, {3589,23.51}, {3589,23.32}, {3590,23.14}, {3588,22.95}, {3586,22.78}, {3587,22.60}, {3585,22.41}, {3585,22.24}, {3586,22.04}, {3583,21.87}, {3584,21.70}, {3583,21.50}, {3583,21.33}, {3583,21.13}, {3581,20.96}, {3581,20.79}, {3580,20.59}, {3580,20.42}, {3578,20.22}, {3578,20.05}, {3576,19.88}, {3577,19.68}, {3576,19.51}, {3574,19.31}, {3573,19.14}, {3573,18.97}, {3573,18.77}, {3570,18.60}, {3569,18.40}, {3571,18.23}, {3567,18.03}, {3567,17.86}, {3567,17.69}, {3567,17.49}, {3565,17.32}, {3564,17.13}, {3563,16.95}, {3561,16.76}, {3561,16.58}, {3560,16.41}, {3558,16.22}, {3559,16.04}, {3557,15.85}, {3556,15.68}, {3554,15.48}, {3552,15.31}, {3554,15.11}, {3553,14.94}, {3551,14.74}, {3550,14.57}, {3548,14.37}, {3585,14.25}, {3588,14.13}, {3590,14.00}, {3588,13.88}, {3588,13.73}, {3589,13.61}, {3586,13.49}, {3587,13.37}, {3586,13.22}, {3585,13.10}, {3585,12.97}, {3583,12.85}, {3583,12.73}, {3583,12.58}, {3581,12.46}, {3581,12.33}, {3580,12.21}, {3578,12.06}, {3579,11.94}, {3577,11.82}, {3576,11.70}, {3577,11.57}, {3577,11.43}, {3575,11.30}, {3575,11.18}, {3576,11.06}, {3574,10.93}, {3574,10.81}, {3574,10.66}, {3573,10.54}, {3572,10.42}, {3573,10.29}, {3571,10.17}, {3571,10.02}, {3570,9.90}, {3571,9.78}, {3569,9.66}, {3570,9.53}, {3569,9.41}, {3569,9.26}, {3568,9.14}, {3567,9.02}, {3567,8.89}, {3567,8.77}, {3567,8.65}, {3566,8.50}, {3567,8.38}, {3566,8.26}, {3564,8.13}, {3565,8.01}, {3564,7.86}, {3563,7.74}, {3562,7.62}, {3563,7.49}, {3562,7.37}, {3561,7.25}, {3560,7.10}, {3560,6.98}, {3559,6.86}, {3557,6.73}, {3556,6.61}, {3556,6.49}, {3554,6.34}, {3554,6.22}, {3552,6.09}, {3550,5.97}, {3548,5.85}, {3547,5.72}, {3546,5.58}, {3544,5.45}, {3543,5.33}, {3541,5.21}, {3537,5.09}, {3535,4.96}, {3533,4.82}, {3529,4.69}, {3527,4.57}, {3522,4.45}, {3517,4.32}, {3513,4.18}, {3509,4.05}, {3503,3.93}, {3499,3.81}, {3495,3.69}, {3488,3.54}, {3482,3.42}, {3477,3.29}, {3470,3.17}, {3463,3.02}, {3458,2.90}, {3449,2.78}, {3442,2.65}, {3435,2.51}, {3426,2.38}, {3420,2.26}, {3410,2.14}, {3400,1.99}, {3393,1.87}, {3383,1.74}, {3374,1.62}, {3363,1.47}, {3352,1.35}, {3340,1.23}, {3328,1.08}, {3316,0.96}, {3304,0.84}, {3399,0.81}};


static int g933_request_battery(hid_device *device_handle);

void g933_init(struct device ** device)
{ 
  device_g933.idVendor = VENDOR_LOGITECH;
  device_g933.idProductsSupported = &PRODUCT_ID;
  device_g933.numIdProducts = 1;

  strcpy(device_g933.device_name, "Logitech G933 Wireless");

  device_g933.capabilities = CAP_BATTERY_STATUS;

  device_g933.request_battery = &g933_request_battery;

  *device = &device_g933;
}

static int g933_request_battery(hid_device *device_handle)
{
  /*
    CREDIT GOES TO https://github.com/ashkitten/ for the project
    https://github.com/ashkitten/g933-utils/
    I've simply ported that implementation to this project!
  */

  int r = 0;
  // request battery voltage
  unsigned char data_request[20] = {0x11,0xFF,0x08,0x0a,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

  
  r = hid_write(device_handle, data_request, 20);
  if (r < 0) return r;

  unsigned char data_read[7];
  r = hid_read(device_handle, data_read, 7);
  if (r < 0) return r;

  //6th byte is state; 0x1 for idle, 0x3 for charging
  unsigned char state = data_read[6];
  if (state == 0x03) return BATTERY_LOADING;

  #ifdef DEBUG
  printf("b1: 0x%08x b2: 0x%08x\n", data_read[4], data_read[5]);
  #endif
  // actual voltage is byte 4 and byte 5 combined together
  const uint16_t voltage = (data_read[4] << 8) | data_read[5];
  
  #ifdef DEBUG
  printf("Reported Voltage: %2f\n", (float)voltage);
  #endif

  float closestDelta,
  closestBatteryLevel,
  closestVoltage;
  
  // return 0 if the voltage is less than the last voltage entry in map
  if (voltage < voltage_map[NUMBER_OF_VMAP_LINES-1][0]) return 0;
  // look for current voltage in the voltage map
  // sets closestBatterylevel to the level entry in the map
  for (int i = 0; i < NUMBER_OF_VMAP_LINES; i++){
    if ((voltage - voltage_map[i][0]) < closestDelta) {
      closestDelta = abs((int)(voltage_map[i][0] - voltage));
      closestBatteryLevel = voltage_map[i][1];
      closestVoltage = voltage_map[i][0];
    }
  }
  
  #ifdef DEBUG
  printf("closest %2f, delta %2f\n", (float)closestVoltage, closestDelta);
  #endif
  return round(closestBatteryLevel);

}