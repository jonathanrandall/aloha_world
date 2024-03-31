import h5py
import os
import xarm
import time
import cv2
import numpy as np

data_dir = "/home/jonny/projects/aloha_world/episodes/wave_hello"

class ReplayEpisodes:
    def __init__(self, data_dir):
        self.data_dir = data_dir
        self.robot = self.connect_to_robot()
    
    def read_file(self, index):
        cam_name = 'top'
        dataset_file = os.path.join(self.data_dir, f'episode_{index}.hdf5')

        with h5py.File(dataset_file, 'r') as root:
            is_sim = root.attrs['sim']
            original_action_shape = root['/action'].shape
            original_qpos_shape = root['/observations/qpos'].shape
            # print('original action shape:', original_action_shape)
            # print('original action shape:', original_qpos_shape)
            # get observation at start_ts only
            qpos = root['/observations/qpos']
            images = []
            images_rec = root[f'/observations/images/{cam_name}']
            for im in images_rec:
                images.append(im)

            actions_rec = root['/action'] #hack, to make timesteps more aligned
            actions = np.zeros(original_action_shape, dtype=np.float32)
            actions[:original_action_shape[0]] = actions_rec

            qpos_rec = root['/observations/qpos'] #hack, to make timesteps more aligned
            qpos = np.zeros(original_qpos_shape, dtype=np.float32)
            qpos[:original_qpos_shape[0]] = qpos_rec

            # print(actions)
        
        return qpos, actions, images
    
    def play_episode(self, index):
        qpos, actions, images = self.read_file(index=index)
        print('qpos shape', qpos.shape)# qpos.shape)
        print('action shape', actions.shape)
        print('images length:', len(images))
        #sometimes there is one less image because of the timing. If this happens append the 
        # last image tot he end
        if (len(images) < actions.shape[0]):
            images.append(images[-1])
        # actions = qpos

        # print(qpos)

        strt_time = time.time()
        time_int = 0.55
        for i, action in enumerate(actions):
            cv2.imshow(f'Image', images[i])
            key = cv2.waitKey(1) & 0xFF
            if key == ord('q'):
                break
            # print(action)
            for k in range(1,7):                
                self.robot.setPosition(k,int(action[k-1]), wait=False)
                time.sleep(0.02)
                # print(self.robot.getPosition(k))
                time.sleep(0.02)
            while (time.time()-strt_time) < time_int:
                pass 
            strt_time = time.time()
        cv2.destroyAllWindows()



    def connect_to_robot(self):
        try:
            arm = xarm.Controller("USB")
            return arm
        except:
            print("Problem connecting to robot. Check connections")
            return None

def main(args=None):
    re = ReplayEpisodes(data_dir=data_dir)
    re.play_episode(25)
    # for i in range(0,12):
    #     re.play_episode(i*3)



if __name__ == '__main__':
    main()
