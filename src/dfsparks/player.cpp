#include "dfsparks/player.h"
#include "dfsparks/log.h"
#include "dfsparks/math.h"
#include <stdlib.h>

namespace dfsparks {

// --------------------------------------------------------------------
// Player
// --------------------------------------------------------------------
Player::Player(Pixels& pixels) : repertoire_(pixels), playlist_(repertoire_), effect_(&playlist_.currentEffect()) {
  pixels_ = &pixels;
}
Player::~Player() { 
}

void Player::render() {
  assert(effect_);
  int32_t curr_ts = timeMillis();
  remaining_time_ -= curr_ts - frame_ts_;
  elapsed_time_ += curr_ts - frame_ts_;
  time_since_beat_ += curr_ts - frame_ts_;
  frame_ts_ = curr_ts;

  if (remaining_time_ <= 0) {
    priority_ = LOW_PRIORITY; // so that server can take over
    switch(mode_) {
      case cycle_all:
        doPlay(playlist_.next(), LOW_PRIORITY);
        break;
      case shuffle_all:
        doPlay(playlist_.random(), LOW_PRIORITY);
        break;
      case play_forever:
        remaining_time_ = effect_->preferredDuration(); 
        break;
    }
  }

  if (specialMode_ > 0) {
    doRenderSpecial();
  }
  else if (showStatus_) {
    doRenderStatus();
  }
  else {
    effect_->render(*this);
  }
}

void Player::doPlay(Effect& ef, int priority) {
  doPlay(ef, priority, 0, ef.preferredDuration(), 0);
}

void Player::doPlay(Effect &ef, int priority, int32_t elapsed, int32_t remaining, uint8_t cycle_hue) {
  if (priority_ > priority) {
    return;
  }

  effect_ = &ef;
  elapsed_time_ = elapsed;
  remaining_time_ = remaining;
  cycle_hue_ = cycle_hue;
  priority_ = priority;

  if (priority_ > LOW_PRIORITY) {
    showStatus_ = false;
  }
  info("player %p selected effect %s", this, effectName());
}

void Player::doRenderStatus() {
  pixels().fill(0x00ff00);
}

void Player::doRenderSpecial() {
  pixels().fill(0x00ffff);
}

const char *Player::effectName() const {
  const char *name = "unnamed";
  repertoire_.find(*effect_, &name, nullptr);
  return name;
}

Effect* Player::findEffect(const char *name) const {
  Effect* ef = nullptr;
  repertoire_.find(name, &ef, nullptr);
  return ef;
}

// --------------------------------------------------------------------
// NetworkPlayer
// --------------------------------------------------------------------

NetworkPlayer::NetworkPlayer(Pixels& pixels, Network &n) : Player(pixels), netwrk(n) {
  netwrk.add(*this);
}

NetworkPlayer::~NetworkPlayer() {
  netwrk.remove(*this);
}

void NetworkPlayer::render() {
  auto curr_time = timeMillis();
  const char *name = effectName();
  if (mode_ == MASTER && name[0] != '_' && (tx_track_ != track() || curr_time - tx_time_ > tx_interval_)) {
    Message::Frame fr;
    strncpy(fr.pattern, name, sizeof(fr.pattern));
    fr.elapsed_ms = timeElapsed();
    fr.beat_ms = timeSinceBeat();
    netwrk.broadcast(fr);
    tx_track_ = track();
    tx_time_ = curr_time;
  }
  Player::render();
}

void NetworkPlayer::doRenderStatus() {
  int32_t t = timeMillis();
  uint32_t color = 0x000000;

  switch(netwrk.status()) {
    case Network::disconnected: 
    case Network::disconnecting: 
      color = 0xff6600; 
      break;
    case Network::connecting: 
      color = t % 500 < 250 ? 0 : 0xff6600;
      break;
    case Network::connected: 
      color = t - rx_time_ < 100 ? 0 : 0x00ff00; 
      break;
    case Network::connection_failed: 
      color = 0xff0000; 
      break;
  }
  pixels().fill(color);
}

void NetworkPlayer::doRenderSpecial() {


  if (specialMode_ > 6) {
    specialMode_ = 1;
  }

  if (specialMode_ == 1) {

    int32_t t = timeMillis();
    const bool blink = ((t % 1000) < 500);
    const int32_t pixelCount = pixels().count();
    const int32_t w = pixels().width();
    const int32_t h = pixels().height();
    for (int i = 0; i < pixelCount; i++) {
      int32_t x, y;
      pixels().coords(i, &x, &y);
      const int32_t black = 0, green = 0x00ff00, blue = 0x0000ff, red = 0xff0000;
      const int32_t yellow = green | red, purple = red | blue, white = 0xffffff;
      const int32_t orange = 0xffcc00;

      int32_t yColors[20] = {0, red, green, blue, yellow,
        purple, orange, white, blue, yellow,
        red, purple, green, orange,
        white, yellow, purple, green, blue, red};

      int32_t col = 0;

      if (y >= 0 && y < 20) {
        col = yColors[y];
      }

      if (x == 0 || x == 6 || x == 14) {
        col &= 0xff7f7f;
      } else if (x == 5 || x == 13 || x == 19) {
        col &= 0x7fff7f;
      }

      if (blink) {
        if (y == 1 &&
            (x == 0 || x == 6 || x == 14)) {
          col = 0;
        } else if (y == 2 &&
                   (x == 5 || x == 13 || x == 19)) {
          col = 0;
        }
      }

      pixels().setColor(i, col);

      //    if (y == 20) {
      //      pixels().setColor(i, 0x00ff00); // green
      //    } else if (y == 19) {
      //      pixels().setColor(i, 0x0000ff); // blue
      //    } else if (y == 18) {
      //      pixels().setColor(i, 0xff0000); // red
      //    } else {
      //      pixels().setColor(i, 0x000000); // black
      //    }

      //    const uint32_t blue = (255 * x / w) & 0xff;
      //    const uint32_t red = (255 * y / h) & 0xff;
      //    pixels().setColor(i, blue | (red << 16));



      //    if (x == 0 || x == w) {
      //      pixels().setColor(i, 0x00ff00); // green
      //    } else if (y == 0 || y == h) {
      //      pixels().setColor(i, 0x0000ff); // blue
      //    } else {
      //      pixels().setColor(i, 0xff0000); // red
      //    }
    }
    return;
  }
#if 0
  const int32_t pixelCount = pixels().count();
  const int32_t greenPixel = (t / 100) % pixelCount;
  for (int32_t i = 0; i < pixelCount; ++i) {
    if (i == greenPixel) {
      pixels().setColor(i, 0x00ff00); // green
    } else {
      pixels().setColor(i, 0xff0000); // red
    }
  }
#endif // 0
  uint32_t color = 0x000000; // black
  switch(specialMode_) {
    case 2:
      color = 0x000000; // black
      break;
    case 3:
      color = 0xffffff; // white
      // color = 0xff00ff; // purple
      break;
    case 4:
      color = 0xff0000; // red
      break;
    case 5:
      color = 0x00ff00; // green
      break;
    case 6:
      color = 0x0000ff; // blue
      break;
#if 0
    case 6: // SOS in Morse Code
      const char *const morseStr = "1010100011101110111000101010000000";
      const uint32_t morseStrLen = sizeof(morseStr) - 1;
      const uint32_t morseDotDurationMS = 250;
      const uint32_t morseIndex = t % (morseDotDurationMS * morseStrLen) / morseDotDurationMS;
      if (morseStr[morseIndex] == '1') {
        color = 0xffffff;
      }
      break;
#endif // 0
  }
  pixels().fill(color);

}

void NetworkPlayer::onReceived(Network&, const Message::Frame& fr) {
  int32_t currTime = timeMillis();
  if (mode_ == SLAVE) {
    Effect *ef = findEffect(fr.pattern);
    if (ef) {
      doPlay(*ef, LOW_PRIORITY, fr.elapsed_ms, rx_timeout_, 0);
    }
    else {
      const char *altname = 0;
      if (fr.hue_dev < 50) {
        altname = "glitter";
        doPlay(*findEffect(altname), LOW_PRIORITY, fr.elapsed_ms, rx_timeout_, fr.hue_med ? fr.hue_med : 255);
      } else {
        altname = "radiaterainbow";        
        doPlay(*findEffect(altname), LOW_PRIORITY, fr.elapsed_ms, rx_timeout_, 0);
      }
      info("Can't play unknown effect '%s', using '%s' instead", fr.pattern, altname);
    }
  }
  rx_time_ = currTime;
}


} // namespace dfsparks
