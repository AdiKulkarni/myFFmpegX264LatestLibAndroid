/*
  Copyright (c) 2012 The WebM project authors. All Rights Reserved.

  Use of this source code is governed by a BSD-style license
  that can be found in the LICENSE file in the root of the source
  tree. An additional intellectual property rights grant can be found
  in the file PATENTS.  All contributing project authors may
  be found in the AUTHORS file in the root of the source tree.
*/

#include "third_party/googletest/src/include/gtest/gtest.h"
#include "test/codec_factory.h"
#include "test/encode_test_driver.h"
#include "test/i420_video_source.h"
#include "test/util.h"

namespace {

const int kMaxPsnr = 100;

class LossLessTest : public ::libvpx_test::EncoderTest,
    public ::libvpx_test::CodecTestWithParam<libvpx_test::TestMode> {
 protected:
  LossLessTest() : EncoderTest(GET_PARAM(0)),
                   psnr_(kMaxPsnr),
                   nframes_(0),
                   encoding_mode_(GET_PARAM(1)) {
  }

  virtual ~LossLessTest() {}

  virtual void SetUp() {
    InitializeConfig();
    SetMode(encoding_mode_);
  }

  virtual void BeginPassHook(unsigned int /*pass*/) {
    psnr_ = kMaxPsnr;
    nframes_ = 0;
  }

  virtual void PSNRPktHook(const vpx_codec_cx_pkt_t *pkt) {
    if (pkt->data.psnr.psnr[0] < psnr_)
      psnr_= pkt->data.psnr.psnr[0];
  }

  double GetMinPsnr() const {
      return psnr_;
  }

 private:
  double psnr_;
  unsigned int nframes_;
  libvpx_test::TestMode encoding_mode_;
};

TEST_P(LossLessTest, TestLossLessEncoding) {
  const vpx_rational timebase = { 33333333, 1000000000 };
  cfg_.g_timebase = timebase;
  cfg_.rc_target_bitrate = 2000;
  cfg_.g_lag_in_frames = 25;
  cfg_.rc_min_quantizer = 0;
  cfg_.rc_max_quantizer = 0;

  init_flags_ = VPX_CODEC_USE_PSNR;

  // intentionally changed the dimension for better testing coverage
  libvpx_test::I420VideoSource video("hantro_collage_w352h288.yuv", 352, 288,
                                     timebase.den, timebase.num, 0, 30);
  ASSERT_NO_FATAL_FAILURE(RunLoop(&video));
  const double psnr_lossless = GetMinPsnr();
  EXPECT_GE(psnr_lossless, kMaxPsnr);
}
VP9_INSTANTIATE_TEST_CASE(LossLessTest, ALL_TEST_MODES);
}  // namespace
