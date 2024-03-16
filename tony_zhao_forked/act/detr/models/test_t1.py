# import sys

# sys.path.append('/home/jonny/projects/aloha_world/tony_zhao_forked/')

# from act.detr.util.misc import NestedTensor

import torch

l1 = []
t1 = torch.ones([2,3])
t2 = torch.ones([4,3])
t2[:,0]=3
# t2 = torch.zeros([5,3])

l1.append(t1)
l1.append(t2)
# print(l1)

l2=(torch.vstack(l1))
print(l2)
l3 = torch.stack([l2])
print(l3.size())
print(l3.mean(dim=[0, 1], keepdim=True))
