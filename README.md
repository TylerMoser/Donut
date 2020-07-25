<h1 align="center">
  <img src="https://github.com/TylerMoser/Donut/blob/master/donutAscii.png" alt="ASCII donut" width="300">
  <img src="https://github.com/TylerMoser/Donut/blob/master/donutPixel.png" alt="pixel donut" width="300">
</h1>

### Introduction
While browsing YouTube, I came across a video titled "[Donut-shaped C code that generates a 3D spinning donut](https://www.youtube.com/watch?v=DEqXNfs_HhY)". This video, uploaded in July 2020, was about [an article](https://www.a1k0n.net/2011/07/20/donut-math.html) posted in 2011. That article was itself about [another article](https://www.a1k0n.net/2006/09/15/obfuscated-c-donut.html) published in 2006. This original article is the origin of the "donut-shaped C code". A slightly-deobfuscated version of the code can be found [here](https://www.dropbox.com/s/79ga2m7p2bnj1ga/donut_deobfuscated.c?dl=0).

Over the course of a few days, I further deobfuscated the code with input from the second article. I also added support for Windows, and made a slight modification to allow the donut to rotate for a much longer period of time before it stops rotating. I also ported the code to Java. The Java version of the code supports the original ascii, as well as a pixel-based output. The original program uses 12 "levels" of illumination, represented by 12 different ascii characters. The pixel-based output uses 128 shades of grayscale. As I originally intended to use this code as a sort of benchmarking exercise, both versions of my code run for a single minute, and then print the average number of frames rendered during each second of that minute.

### Instructions
The C version of the code can be compiled with `gcc -Wall -O2 donut.c -lm`. The resulting object file can then be executed.

On Windows, the C code can also be compiled and executed using visual studio. To do so, create a new C++ console application, delete all of the auto-generated files, and place donut.c in the "Source Files" folder. Then turn off precompiled headers by right-clicking on the project in the solution explorer and clicking `properties` > `Configuration Properties` > `C/C++` > `Precompiled Headers` and changing the "Precompiled Header" drop-down to `"Not Using Precompiled Headers"`.

The Java code can be compiled from this repository's base directory by running `javac ./me/tylermoser/Donut.java`. It can then be executed with `java me.tylermoser.Donut`.

### Background
The YouTube video immediately grabbed my attention as a potential short-but-interesting project opportunity. I have always been interested in learning some of the math behind 3D graphics, and I was hoping that the explanation provided by the second article might be a good place to start. Like most graphics workloads, the code is heavily parallel and the bulk of the computation is single-precision floating-point operations. I saw this as an opportunity to return to one of my favorite topics: hardware acceleration. I have been on the lookout for a project that could give me an excuse to return to FPGA programming or to learn GPU programming for quite some time. I had also been looking for an original way to further explore the performance impacts of coroutines. Given that this code provided a highly-parallel workload, it checked all of my boxes. My original idea was to port the code to several languages/frameworks (including Verilog and CUDA). I would then use frames-per-second as a benchmarking metric for various levels of parallelization.

However, digging into the code and its associated explanation didn't go quite like I had hoped. My first guess, when initially looking at the code, was that it rendered the ascii donut character-by-character. Instead, the code generates points in 3D space and maps those points to 2D coordinates on the screen. If enough points are generated, the object appears solid. This means that for most pixels/characters, multiple 3-dimensional points are getting mapped to a single 2d coordinate. The code reconciles this by checking each 3D point and only drawing the point that is closest to the camera for each 2D coordinate. This portion of the code is synchronous.

By making this final stage of rendering synchronous, the code was suddenly a much less ideal example of a parallelizable workload. While the calculations could still be done in parallel, this final stage would require non-trivial data structures and/or locking mechanisms. These structures/mechanisms would almost certainly differ between platforms, and therefore would make "benchmarks" much less reliable.
