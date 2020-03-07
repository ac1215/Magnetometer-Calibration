static float magCalibration[19];

calibration::calibration(){}

uint8_t calibration::initCalibration()
{
	uint8_t status;
	status = imuCalibration.begin();
	if (status == 0)
	{
		Serial.println("NXPMotionSense initialized for calibration");
	}
	else
	{
		Serial.println("NXPMotionSense is not initialized");
		return 1;
	}

	raw_data_reset();
	return 0;
}

void calibration::magneticCalibration(void)
{
	int ax, ay, az;
	int gx, gy, gz;
	int mx, my, mz;
	int32_t i32data[9];
	static uint8_t u8dispCount = 0;

	if (imuCalibration.available()) 
	{
		imuCalibration.readMotionSensor(ax, ay, az, gx, gy, gz, mx, my, mz);
		i32data[0] = ax;
		i32data[1] = ay;
		i32data[2] = az;
		i32data[3] = gx;
		i32data[4] = gy;
		i32data[5] = gz;
		i32data[6] = mx;
		i32data[7] = my;
		i32data[8] = mz;

		if (raw_data(i32data))
		{
			u8dispCount++;
		}

		fGaps = quality_surface_gap_error();
		fVariance = quality_magnitude_variance_error();
		fWobble = quality_wobble_error();
		fFiterror = quality_spherical_fit_error();

		if ((fGaps < GAPS_THRESHOLD) && (fVariance < VARIANCE_THRESHOLD) && (fWobble < WOBBLE_THRESHOLD) && (fFiterror < FITERROR_THRESHOLD)){
			show_calibration_confirmed = true;
		}
		else
		{
			//display_callback function should not call every time.
			if (u8dispCount == QUALITY_UPDATE)
			{
				display_callback();
				u8dispCount = 0;
			}
		}

		if (show_calibration_confirmed)
		{
			getMagcalibration(magCalibration);
			writeCalibration(magCalibration);
		}
	}
}

void calibration::writeCalibration(const void *data)
{
	const uint8_t *p = (const uint8_t *)data;
	uint8_t i;

	for (i = 0; i < MAG_CAL_SIZE; i++)
	{
		EEPROM.write(MAG_CAL_EEADDR + i, p[i]);
	}
	/* Calibration status is write to 1 because
	   When device is not calibrated at the time
	   we need to supply zero value of offseted. */
	EEPROM.write(MAG_CAL_STATUS, 1);
}

bool calibration::calibrationStatus()
{
	if(show_calibration_confirmed){
		return true;
	}
	return false;
}

void calibration::printCalibrationData(void)
{
	Serial.println("####################################");
	Serial.println("Parameters of MotionCal.exe");
	Serial.println(("Gaps: " + String(fGaps));
  	Serial.println("Variance: " + String(fVariance));
  	Serial.println("Wobble: " + String(fWobble));
  	Serial.println("Fit Error: " + String(fFiterror));
	Serial.println("####################################")
	Serial.println("Hard-Iron Effects")
	for(int i = 0; i < 3; i++)
	{
		Serial.print(magcal.V[i]);
	}
	Serial.println();
	Serial.println("####################################")
	Serial.println("Soft-Iron Effects");
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			Serial.print(magcal.invW[i][j]);
		}
		Serial.println();
	}
	Serial.println();
	Serial.println("####################################")
}

calibration::~calibration(){}
