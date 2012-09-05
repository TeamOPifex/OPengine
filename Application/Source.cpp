#include <stdio.h>

#include "Human\Renderer.h"

using namespace OPifex;

int main(){
	printf("Program Started.");

	Renderer::Initialize();
		
	do{
		Renderer::clear_color(1.0f, 0.0f, 0.0f);
	}
	while(Renderer::escape());
		
	printf("Program Ended.");
	return 0;
}
