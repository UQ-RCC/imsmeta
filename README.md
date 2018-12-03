# Dragonfly Metadata to JSON converter

Converts metadata files from the Andoir Dragonfly to JSON.

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

<img align="right" src="http://opensource.org/trademarks/opensource/OSI-Approved-License-100x137.png">

This project is licensed under the [MIT License](http://opensource.org/licenses/MIT):

Copyright &copy; 2018 [The University of Queensland](http://uq.edu.au/)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

* * *

This project uses the [`JSON for Modern C++`](https://github.com/nlohmann/json) library by Niels Lohmann which is licensed under the [MIT License](http://opensource.org/licenses/MIT) (see above). Copyright &copy; 2013-2018 [Niels Lohmann](http://nlohmann.me/)
