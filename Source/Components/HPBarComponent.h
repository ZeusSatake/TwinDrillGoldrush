#include "../../Component.h"
#include "../Components/StatusComponent.h"
#include <memory>
#include <string>

class Enemy;
class Player;
namespace ML { class Vec2; }
namespace DrawGauge { class Object; }

class HPBarComponent : public Component
{
	std::shared_ptr<DrawGauge::Object> bar_;
	const StatusComponent& status_;//ì«Ç›éÊÇËêÍóp

	void SetHPPercentage();

public:
	HPBarComponent(class Enemy* owner);
	HPBarComponent(class Player* owner);
	virtual ~HPBarComponent();

	void Update() override;

	void SetImg(const std::string& path);
	void SetDrawSize(const int width, const int height);
	void SetPos(const ML::Vec2& pos);
	void SetPos(const float x, const float y);
	void SetBackSrc(const ML::Box2D& backSrc);
	void SetInsideSrc(const ML::Box2D& insideSrc);

	void SetSupportScroll(const bool isSupportScroll);
	void SetVisible(const bool visible);

	ML::Point GetSize() const;
};