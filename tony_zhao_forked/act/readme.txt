python3 act/imitate_episodes.py --task_name wave_hello --ckpt_dir '/home/jonny/projects/aloha_world/ckpt_dir_3' --policy_class ACT --kl_weight 10 --chunk_size 50 --hidden_dim 512 --num_epochs 4000 --batch_size 4 --seed 42 --lr 1e-5 --dim_feedforward 512


python3 act/imitate_episodes.py --task_name wave_hello --ckpt_dir '/home/jonny/projects/aloha_world/ckpt_dir_3' --policy_class ACT --kl_weight 10 --chunk_size 50 --hidden_dim 512 --num_epochs 7000 --batch_size 16 --seed 42 --lr 1e-5 --dim_feedforward 512 --eval

python3 act/imitate_episodes.py --task_name wave_hello --ckpt_dir '/home/jonny/projects/aloha_world/ckpt_dir_4' --policy_class ACT --kl_weight 10 --chunk_size 50 --hidden_dim 512 --num_epochs 3500 --batch_size 16 --seed 42 --lr 1e-5 --dim_feedforward 512 --continue_training --historical_length 8 

ckpt_dir with 2000 iterations
ckpt_dir_2 with 10000 iterations action = root['/action'][(start_ts+0):] works best batch_size 4

train ckpt_dir_3 with 7000 iterations action = root['/action'][(start_ts+0):] batch_size 16

train ckpt_dir_4 with 8000 iterations action = root['/action'][(start_ts+0):] batch_size 4 and using 8 steps of history.

train ckpt_dir_5 with 7000 iterations action = root['/action'][(start_ts+0):] batch_size 4 and using 1 steps of history.
