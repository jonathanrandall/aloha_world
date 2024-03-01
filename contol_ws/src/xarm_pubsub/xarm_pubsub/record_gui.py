

import argparse
import random
import signal
import sys
import threading

import rclpy


# from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QVBoxLayout, QMainWindow

from python_qt_binding.QtWidgets import QMainWindow
from python_qt_binding.QtWidgets import QPushButton
from python_qt_binding.QtWidgets import QVBoxLayout
from python_qt_binding.QtWidgets import QWidget
from python_qt_binding.QtWidgets import QApplication

from .puppet_subscriber import MinimalSubscriber

class puppet_gui(QMainWindow):
    def __init__(self, title, ps):
        super(puppet_gui, self).__init__()
        self.setWindowTitle(title)


        # Button for randomizing the sliders
        self.start_button = QPushButton('Start', self)
        self.start_button.clicked.connect(self.start_episode)

        # Button for centering the sliders
        self.stop_button = QPushButton('Stop', self)
        self.stop_button.clicked.connect(self.stop_episode)

        # Main layout
        self.main_layout = QVBoxLayout()

        # Add buttons and scroll area to main layout
        self.main_layout.addWidget(self.start_button)
        self.main_layout.addWidget(self.stop_button)

        # central widget
        self.central_widget = QWidget()
        self.central_widget.setLayout(self.main_layout)
        self.setCentralWidget(self.central_widget)
        
        # self.thread_gui = threading.Thread(target=self.root.mainloop(), daemon=False)
        self.ps=ps

        self.strt_ep = False
        self.stp_ep = False

        self.record_params = ps.record_params

        self.running = True

    def stop_episode(self):
        self.record_params["stop"]=True
        self.record_params["start"] = False
        # Self.strt_ep = False
        # self.stp_ep = True
    
    def start_episode(self):
        # global strt_ep
        # self.ps.strt_ep = True
        self.record_params["start"] = True

    def closeEvent(self, event):
        self.running = False

    def loop(self):
        while self.running:
            rclpy.spin_once(self.ps, timeout_sec=0.1)


def main():
    # Initialize rclpy with the command-line arguments
    rclpy.init()

    # Strip off the ROS 2-specific command-line arguments
    stripped_args = rclpy.utilities.remove_ros_args(args=sys.argv)
    parser = argparse.ArgumentParser()
    # parser.add_argument('urdf_file', help='URDF file to use', nargs='?', default=None)

    # Parse the remaining arguments, noting that the passed-in args must *not*
    # contain the name of the program.
    parsed_args = parser.parse_args(args=stripped_args[1:])

    app = QApplication(sys.argv)
    jsp_gui = puppet_gui('Episode Recorder',
                                     MinimalSubscriber())

    jsp_gui.show()

    threading.Thread(target=jsp_gui.loop).start()
    signal.signal(signal.SIGINT, signal.SIG_DFL)
    sys.exit(app.exec_())

if __name__ == '__main__':
    main()
