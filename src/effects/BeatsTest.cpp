#include "BeatsTest.h"
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include "Palette.h"


extern MyPixelBus * strip;
extern NeoPixelAnimator * animator;

using namespace helperfunc;

bool BeatsTest::InitVars()
{
	addVar(new Variable<uint8_t>("brightness", 100));
	//addVar(new Variable<uint8_t>("filter", 80));
	//addVar(new Variable<float>("beatsratio", 2.1));
	//addVar(new Variable<uint8_t>("beatstimeout", 100));
	addVar(new Variable<EQ*>(1000, 3000)); //  EQ(samples, sampletime) //  sets the defaults for EQ beat detection...  Start with no params for just graphic equaliser settings.

	_EQ = getVar<EQ*>("EQ");


}

bool BeatsTest::Start()
{
	if (strip) {
		strip->ClearTo(0);
	}

	if (animator) {
		delete animator;
		animator = nullptr;
	}

	animator = new NeoPixelAnimator(7);

	if (_EQ) {

		_EQ->SetBeatCallback( [this](EQParam params) {

			//if (params.channel == ) {
			//	Serial.printf("[Packet rec %u: Packet sent %u] channel: %u, avg: %u level: %u, bpm: %u\n", _EQ->seq(), params.seq_no , params.channel, params.average, params.level, params.bpm);
			//}

			uint8_t channel = params.channel;
			uint8_t level = params.level;


			AnimUpdateCallback animUpdate = [ channel, level ](const AnimationParam & aniparam) {
				// apply a exponential curve to both front and back
				float progress = aniparam.progress;
				// lerp between Red and Green
				RgbColor updatedColor;
				RgbColor targetColor = RgbColor(0, 0, 255);
				updatedColor = RgbColor::LinearBlend(targetColor, 0,  progress) ;
				//}
				// in this case, just apply the color to first pixel
				strip->SetPixelColor(  channel  , updatedColor);
			};

			animator->StartAnimation(channel, level,  animUpdate);

		});
	}
}

bool BeatsTest::Run()
{


	if (_EQ) {
		_EQ->loop();
	}

	// if (millis() - _tick > 30) {

	// 	for (int i = 0; i < 7; i++) {
	// 		int LEDs = map(_EQ->data[i], 80, 1023, 0, 8);
	// 		for (int j = 0; j < 8; j++) {
	// 			strip->SetPixelColor( (i * 8) + j, RgbColor(0));
	// 		}
	// 		for (int j = 0; j < LEDs; j ++) {
	// 			strip->SetPixelColor( (i * 8) + j, dim( RgbColor(50, 0, 0), brightness()) );
	// 		}
	// 	}

	// 	_tick = millis();
	// }

}

bool BeatsTest::Stop()
{

	if (strip) {
		strip->ClearTo(0);
	}

	if (animator) {
		delete animator;
		animator = nullptr;
	}
}


