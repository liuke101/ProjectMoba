#pragma once

#include "CoreMinimal.h"
#include "UI/MobaUIBase.h"
#include "UI_NativeOnDrop.generated.h"

/**
 * 捕捉拖拽事件，UI布满全屏
 */
UCLASS()
class PROJECTMOBA_API UUI_NativeOnDrop : public UMobaUIBase
{
	GENERATED_BODY()

protected:
	//拖拽响应
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	
};
