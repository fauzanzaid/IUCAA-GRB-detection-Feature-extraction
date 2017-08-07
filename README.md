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