#pragma once

struct QueueFamilyIndices
{
	int graphicFamily = -1; 

	bool isValid()
	{
		return graphicFamily >= 0;
	}
};
