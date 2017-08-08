# IUCAA-GRB-detection-Feature-extraction

This application reads several light curve files, isolates signals, performs discrete wavelet transform on the isolated signals, and outputs a single file containing normalized features for all the signals.

## Set up

Download this repository. Run ```make``` command from the terminal in the directory. This will create ```a.out``` in the same directory, which you can execute from the terminal.

## Usage

To use the application, run this from the terminal:

```
./a.out [options] lightcurve_directory output_file
```

```lightcurve_directory``` should contain any number of lightcurve files whose features need to be extracted. There should not be any other file inside the directory.

```output_file``` will contain all the normalized discrete wavelet transforms for all the signals which get isolated from the lightcurve files.

### Input file format

```
<n> <signal_class>
<count bin 0>
<count bin 1>
.
.
<count bin n-1>
```

```n``` is an integer which denotes the number of bins. ```signal_class``` is an integer which denotes the class of the signal in the lightcurve. This is only applicable if you need to extract labeled features for training a model. Other wise you can keep it 0.

Then, ```n``` integers follow, which denote the count of photons per bin.


### Output file format

The output file is compatible with LIBSVM format. Each line also contains a comment to convey additional information:

```
% lightcurve_filename lightcurve_idx signal_idx signal_class signal_start_bin signal_len
```

* ```lightcurve_filename``` is the lightcurve file to which the signal belongs.

* ```lightcurve_idx``` is the internally assigned index of the lightcurve file, which can be used for correlating with optional outputs. (Starts from 0)

* ```signal_idx``` is the index of the isolated signal from the lightcurve. Useful if a lightcurve contains more than one signal. (Starts from 0)

* ```signal_class``` is the same as the ```signal_class``` from the lightcurve from which the signal is isolated.

* ```signal_start_bin``` is the zero indexed first bin of the isolated signal.

* ```signal_len``` is the number of bins spanned by the signal.


### Options

* ```--numsignals``` or ```-m```: Specify the number of lightcurves which should be processed, in no particular order. It is useful to check with a few lightcurves to make sure your set up is running as you wish before processing all the lightcurves. If unspecified, all lightcurves are processed.

* ```--peakdir``` or ```-p```: Specify a directory to output the isolated signals. The naming of these files is of the format ```a_b.txt```, where ```a``` is ```lightcurve_idx``` and ```b``` is ```signal_idx```. If unspecified, no signal files are output.

* ```--threshold``` or ```-h```: Specify a threshold parameter. A larger value will isolate only stronger or taller signals. A smaller value will also isolate weaker or lower signals, but may be susceptible to noise. It is important to play with this value for best results. If unspecified, the default value of ```2.0``` is assigned.

* ```--maxzero```: Specify the maximum gap between two isolated regions to be ignored. If regions are closer, they are merged, otherwise they are treated as separate signals. Must be larger than or equal to ```2```. If unspecified, the default value of ```4``` is assigned.

* ``--minlength``` or ```-l```: Specify the minimum length of signal to pick up. Shorter signals are ignored. Must be larger than or equal to ```4```. If unspecified, the default value of ```12``` is assigned.

* ```--padratio``` or ```-r```: Specify amount of padding to add to the signal on either side. If unspecified, the default value of ```0.5``` is assigned.

* ```--dwtdir``` or ```-d```: Specify a directory to output the unnormalized DWT of the isolated signals. The naming of these files is of the format ```a_b.txt```, where ```a``` is ```lightcurve_idx``` and ```b``` is ```signal_idx```. If unspecified, no DWT files are output.

* ```--dwtminlen```: Specify the minimum length to which signals must be scaled to perform DWT. Must be a power of ```2```, larger than or equal to ```4```. If unspecified, the default value of ```16``` is assigned.

* ```--dwtmaxlen```: Specify the maximum length to which signals must be scaled to perform DWT. Must be a power of ```2```. If unspecified, the default value of ```512``` is assigned.

* ```--transform``` or ```-t```: Specify the type of DWT to be performed. Can be ```haar```, ```D4```, ```D6``` or ```D8```.

* ```--append```: If the application run with this option, the output file is appended to, instead of being created anew.

* ```--numfeatures``` or ```-n```: Specify the number of features to output in ```output_file```. If unspecified, the default value of ```7``` is assigned.

## Included tools

In the ```tools``` directory, a python script ```fitsevt.py``` is included to convert CZTI event files to compatible lightcurve files which can be used directly as input files.

Before using this tool ensure that ```astropy``` package is installed on the system. If not already installed, run:

```
pip3 install astropy
```

To use the tool, execute it from the terminal:
```
python3 fitsevt.py input_dir output_dir energy_lo energy_hi bin_size
```

* ```input_dir```: This directly should contain all event files which need to be converted and nothing else.

* ```output_dir```: The script will output the converted files to this directory.

* ```energy_lo```: The lower energy limit of photons to consider, in keV.

* ```energy_hi```: The higher energy limit of photons to consider, in keV.

* ```bin_size```: The size of bins that will hold the count, in seconds.


## Issues

The implementations of the daubechies DWT are buggy and should not relied upon. Specifically, changing the order of coefficients in ```DaubCoef.h``` might solve the problem.