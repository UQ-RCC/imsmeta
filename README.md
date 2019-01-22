# Dragonfly Metadata to JSON converter

Converts metadata files from the Andor Dragonfly to JSON.

## Usage
```bash
imsmeta < meta.txt | json_pp
```

This is designed to run as part of an existing software pipeline, so input/output to files is not supported.

Use shell redirection if you need this functionality.

## Sample Input
```ini
DateAndTime=2018-05-24 10:38:16
NumberOfTimePoints=1
NumberOfZPoints=1
NumberOfChannels=3
Width=2048
Height=2048
FusionVersion=1.5.0.7
[Channels In Image]
	[Channel]
		Description=
		Name=405_DAPI_Confocal_25um_Cam1
		[ImagingMode]
			Name=Confocal_25um
			Modality=SpinningDiskConfocal
			IsConfigurable=True
			IsChannelAssignable=True
			Description=
			IsActive=True
			IsReadOnly=True
			[FeatureValueList]
				{DisplayName=Rotation, Value=0˚}
				{DisplayName=Rotation, Value=0˚}
				{DisplayName=Flip Vertical, Value=True}
```

## Sample Output
```json
{
   "FusionVersion" : "1.5.0.7",
   "NumberOfZPoints" : "1",
   "Height" : "2048",
   "NumberOfChannels" : "3",
   "Channels In Image" : {
      "Channel" : {
         "ImagingMode" : {
            "Name" : "Confocal_25um",
            "IsActive" : "True",
            "Description" : "",
            "IsConfigurable" : "True",
            "IsReadOnly" : "True",
            "IsChannelAssignable" : "True",
            "Modality" : "SpinningDiskConfocal",
            "FeatureValueList" : [
               {
                  "Value" : "0˚",
                  "DisplayName" : "Rotation"
               },
               {
                  "Value" : "0˚",
                  "DisplayName" : "Rotation"
               },
               {
                  "Value" : "True",
                  "DisplayName" : "Flip Vertical"
               }
            ]
         },
         "Description" : "",
         "Name" : "405_DAPI_Confocal_25um_Cam1"
      }
   },
   "DateAndTime" : "2018-05-24 10:38:16",
   "NumberOfTimePoints" : "1",
   "Width" : "2048"
}
```


## License
This project is licensed under the [Apache License, Version 2.0](https://opensource.org/licenses/Apache-2.0):

Copyright &copy; 2019 [The University of Queensland](http://uq.edu.au/)

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

* * *

This project uses the [`JSON for Modern C++`](https://github.com/nlohmann/json) library by Niels Lohmann which is licensed under the [MIT License](http://opensource.org/licenses/MIT) (see above). Copyright &copy; 2013-2018 [Niels Lohmann](http://nlohmann.me/)
