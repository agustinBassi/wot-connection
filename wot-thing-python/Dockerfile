# The image to start
FROM python:3.6-alpine
RUN mkdir /app
ENV PYTHONPATH $PYTHONPATH:/app
# set /app as a working dir
WORKDIR /app
# copy dependencies file into container
COPY src/requirements.txt /app
# install python dependencies
RUN python3 -m pip install -r requirements.txt --no-cache-dir
# set the port that will be shared outside the container
EXPOSE 8888
