
==============================================================================

Draytek Digistat RF3 uses short on, long on, short gap and long gap. It begins each pulse train by sending a long on (488 Samples long)

ON
488 Samples 1 1 1 2  1 1 2 1 1  1 2 1  2  1 1 1 2  1 1 1 1 2 1  1 1 2
OFF
488 Samples 1 1 1 2  1 1 2 1 1  1 2 1  2  1 1 1 1 1 1 1 1 2  2  1 1 2

1 is 36 samples long =  0.000816326530612  816uS
2 is 55 samples long =  0.00124716553288  1247uS
gap  10 samples long =  0.000226757369615  227uS
lgap 33 samples long =  0.000748299319728  748uS
488 Samples          =  0.0110657596372  11066uS 

221062 samples = 5.013 seconds

44100 Hz = 44100 samples per second

==============================================================================
