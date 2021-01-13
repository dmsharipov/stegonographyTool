# stegonographyTool
An encoder program that implements steganographic hiding of the "secret" text file, and a decoder program that extracts the "secret" message from the stegocontainer.
Works only with Russian alphabet and .bmp files.

# Encoder
The encoder program implements steganographic hiding of the "secret" text file into the created graphic file. To hide the text, the method of replacing the least significant bits in the global palette and in the content of the graphic file was applied according to the algorithm: in each byte of the palette, the least significant bit is changed by the bit of the encrypted message and the resulting byte is written to the resulting file. You can select the degree of packing from 1 to 7 bits.

# Decoder
The decoder program extracts the "secret" message from the stegocontainer by extracting the least significant bits in the content part of the graphic image and then converting the extracted bits into text.