#include "UIComponent.h"

void UIComponent::render(){
	render(0.0);
}

void UIComponent::render(float distance){
	
}

void UIComponent::update(double elapsed_time){
	Matrix44 _model = getGlobalMatrix();
	//por el momento solo da target siendo nesteado en una nave
	if(uiElement!=NULL && uiElement->ttl > 0 ){
		uiElement->pos =Vector3(_model.m[12], _model.m[13], _model.m[14]);
		uiElement->size = size;
	}else{
		uiElement = ui->createUI(Vector3(_model.m[12], _model.m[13], _model.m[14]), "panel");
	}
	

}