# IUCAA-GRB-detection-Feature-extraction

This application reads several light curve files, isolates signals, performs discrete wavelet transform on the isolated signals, and outputs a single file containing normalized features for all the signals.

## Set up

Downoad this repository. Run ```make``` command from the terminal in the directory. This will create ```a.out``` in the same directory, which you can execute from the terminal.

## Usage

To use the application, run this from the terminal:

```
./a.out [options] signal_directory output_file
```

```signal_directory``` should contain any number of lightcurve files whose features need to be extracted. There should not be any other file inside the folder.

```output_file``` will contain all the normalized discrete wavelet transforms for all the signals which get isolated from the lightcurve files.

### Input file format

```
<n> <signal-class>
<count bin 1>
<count bin 2>
.
.
<count bin n>
```

```n``` is an integer which denotes the number of bins. ```signal-class``` is an integer which denotes the class of the signal in the lightcurve. This is only applicable if you need to extract labelled features for training a model. Other wise you can keep it 0.

Then, ```n``` integers follow, which denote the count of photons per bin.