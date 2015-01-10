# FFMpeg-encode-example

Example how to use ffmpeg to decode video file. Link to article about [encode with FFMpeg](http://unick-soft.ru/article.php?id=57). [Russian Article](http://unick-soft.ru/article.php?id=20)

Program creates video with sound. Here is example settings:

<pre>
// Frame size.
#define W_VIDEO 320
#define H_VIDEO 240
// Output file name.
#define FILE_NAME          "c:\\temp\\1.avi"
// Cound of frames in output file.
#define FRAME_COUNT        150
// Container.
#define CONTAINER          "auto"
</pre>

CONTAINER - which container will we use. "auto" - use file extention. Another value is: "avi", "mp4", "mpeg", "wmv", "mov".
