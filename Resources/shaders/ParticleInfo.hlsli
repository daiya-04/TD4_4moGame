
struct Particle {
    float32_t3 translate;
    float32_t3 scale;
	float32_t3 rotate;
	float32_t3 velocity;
    float32_t lifeTime;
    float32_t currentTime;
    float32_t4 color;
    uint32_t billboardType;
    float32_t3 roringSpeed;
    float32_t3 noiseOffset;
};

struct OverLifeTime {
    uint32_t isConstantVelocity;
    float32_t3 velocity;

    uint32_t isTransVelocity;
    float32_t3 startVelocity;
    float32_t3 endVelocity;

    uint32_t isScale;
    float32_t startScale;
    float32_t endScale;

    uint32_t isColor;
    float32_t3 startColor;
    float32_t3 endColor;

    uint32_t isAlpha;
    float32_t startAlpha;
    float32_t midAlpha;
    float32_t endAlpha;

    uint32_t isTransSpeed;
    float32_t startSpeed;
    float32_t endSpeed;

    float32_t gravity;

    uint32_t isRoring;
    float32_t3 minRoringSpeed;
    float32_t3 maxRoringSpeed;

    uint32_t isNoise;
    float32_t3 density;
    float32_t strength;
    uint32_t isRandom;
};