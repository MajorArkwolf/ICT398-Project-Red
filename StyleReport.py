# Use modern subprocess libraries
import subprocess
import sys

# Set the (order-critical) configuration flags for running cpplint on this repository
config = [sys.executable]
config.append("./lib/cpplint/cpplint.py")
config.append("--verbose=0")
config.append("--output=emacs")
#config.append("--filter=")
config.append("--counting=detailed")
config.append("--root=..")
config.append("--repository=")
config.append("--linelength=80")
config.append("--headers=hpp,h")
config.append("--recursive")
#config.append("--exclude=")
config.append("--extensions=hpp,cpp,h,c")
config.append("--includeorder=default")
config.append("./src/")

# Run cpplint with the above configuration
try:
  # An error will be thrown if cpplint could not be executed
  subprocess.run([config[0], config[1], "--version"])
  subprocess.run(config)
except OSError as err:
  # Catch and report the error
  print("Failed to execute \"./cpplint.py\": ", err)