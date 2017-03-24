#include <stdlib.h>
#include <stdio.h>

#include "cover.h"
#include "list.h"
#include "set.h"
/* 从所给的子集的集合subsets中寻找最小的子集组合Covering，使其能够覆盖全集members */
/* args:
    members: 所要覆盖的目标集合
    subsets: 由members的子集所组成的集合
    covering: 寻找的最小子集集合
   method:
   每次从subsets中寻找最大覆盖率的子集，找到后加入到covering集合，
   从members中删除已覆盖的元素，从subsets从删除已加入到covering中的子集元素
*/
int cover(Set *members, Set *subsets, Set *covering)
{
    Set intersection;
    KSet *subset;
    ListElmt *member,
             *max_member;
    void *data;
    int max_size;

    set_init(covering, subsets->match, NULL);

    while(set_size(members) > 0 && set_size(subsets) > 0)
    {
        max_size = 0;
        /* 找到覆盖率最大的子集 */
        for(member = list_head(subsets); member != NULL; member = list_next(member))
        {
            if(set_intersection(&intersection, &((KSet *)list_data(member))->set, members) != 0)
            {
                return -1;
            }
            if(set_size(&intersection) > max_size)
            {
                max_member = member;
                max_size = set_size(&intersection);
            }
            /* 只是寻找覆盖率最大的子集，并不使用交集 */
            set_destroy(&intersection);
        }

        //没有交集，说明无法覆盖余下的members元素,因此cover失败
        if(max_size == 0)
            return 1;

        //对找到的最大覆盖率子集进行处理
        subset = (KSet *)list_data(max_member);
        //step1: 将子集加入到covering中
        if(set_insert(covering, subset) != 0)
            return -1;
        //从members,subsets中删除子集元素
        for(member = list_head(&((KSet *)list_data(max_member))->set); member != NULL; member = list_next(member))
        {
            data = list_data(member);
            //step2: 从members中删除元素
            if(set_remove(members, (void **)data) == 0 &&members->destroy != NULL)
                members->destroy(data);
            //step3: 从subsets中删除元素
            if(set_remove(subsets, (void **)&subset) != 0)
                return -1;
        }


    }
    //如果没覆盖完，则表明无法覆盖
    if(set_size(members) > 0)
        return -1;
    return 0;
}
