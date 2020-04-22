/**************************************************************************
**	二分查找法
**	在有序数组中查找target，查找到返回index，否则返回-1
*************************************************************************/
#include <iostream>
#include <vector>

using namespace std;


//循环方法
int binary_search1(vector<int> &nums,int target)
{
	int left = 0;
	int right = nums.size() - 1;

	while (left <= right)
	{
		int mid = (right - left) / 2 + left;
		if (nums[mid] > target)
		{
			right = mid-1;
		}
		else if (nums[mid] < target)
		{
			left = mid+1;
		}
		else
		{
			return mid;
		}
	}
	return -1;
}

//递归方法
int binary_search2(vector<int> &nums, int left, int right, int target)
{
	if (left > right) return -1;
	 
	int mid = (right - left) / 2 + left;

	if (nums[mid] == target)
	{
		return mid;
	}
	else if (nums[mid] > target)
	{
		return binary_search2(nums, left, mid-1, target);
	}
	else
	{
		return binary_search2(nums, mid+1, right, target);
	}
}

int main()
{
	vector<int> nums = { 1,2,3,4,5,6,7 };
	cout << binary_search2(nums, 0, nums.size(), 1) << endl;
	return 0;
}
