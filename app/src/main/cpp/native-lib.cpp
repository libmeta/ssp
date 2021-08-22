#include <jni.h>
#include <string>
#include <unistd.h>

#include "Base/fps.hpp"
#include "Base/bitrate.hpp"

extern "C" JNIEXPORT jstring JNICALL
Java_com_pzw_ssp_MainActivity_stringFromJNI(
	JNIEnv *env,
	jobject /* this */) {
  std::string hello = "Hello from C++";

  ssp::FPS fps;
  float f = 0.f;
  while (!fps.update(f)) {
	usleep(23'000);
  }

  hello += (" fps = " + std::to_string(f));

  ssp::Bitrate bps;
  uint64_t b = 0;
  uint64_t c = 10000;
  while (!bps.update(b, c)) {
	usleep(23'000);
	c += 50000;
  }

  hello += (" bps = " + std::to_string(b));
  return env->NewStringUTF(hello.c_str());
}