Smart Rain Gauge Project
========================

The Smart Rain Gauge project is a distributed system of Wireless sensing nodes that monitor precipitation in the Syracuse, NY region. This repository has all the necessary code and documentation needed for setting up this system.


Web Application
---------------

This is an application written in primarily Python that is responsible for storing and displaying data from the Particle Electron devices in a web interface. The code necessary for this is currently in a private repository and will be moved here shortly. Please excuse any inconvience. 

### Technology Used

**Application**

- **Flask** - The Python web framework being used. There are also a number of other third party dependencies, also written in Python and installed with `pip`.
- **npm** - Node Package Manager. `pip` for Node.js
- **gulp** - Build system, written in Javascript. Automates repetitive development & build tasks.
- **bower** - Front end dependency management (i.e. bootstrap, etc.)

**Server**

- **MySQL/MariaDB** - Database housing all of the information for the application (and sensors)
- **nginx** - HTTP server (like Apache)
- **gunicorn** - Python WSGI server (runs our Python application)

### Architecture Overview

The Electron cloud has an active webhook listening for a specific `publish()` event from any Electron device. When the publish event runs every 15 minutes, the webhook sends an HTTP POST containing formatted JSON to a regular HTTP endpoint in our application, which processes and inserts the data into the MySQL database.

The web application provides user and access management, searchable raw data, maps, more information, and much more.

### Local Setup

This application runs locally for development & testing, and is also deployed to the production machines locally via ssh.

#### Prereqresites

You need the following installed on your machine before we get started. There should be installers on the official websites:

- Python 2.7 _(2.7.10 or above preferred)_

- Node.js (for dev/builds + front end dependencies)

- MySQL or MariaDB

#### Set up (first time only)

**Database**

Make sure that MySQL is running locally first, and create an empty database named **savetherain** (`create database savetherain;` from `mysql` shell). 

After the database is created, run `python manage.py db migrate` followed by `python manage.py db upgrade`. This should populate your formerly empty database with the proper database schema.

**Virtualenv**

We use a tool called _virtualenv_ to keep a self-contained Python environment so that we don't have to install dependencies on our system level. To install it, just run:

`pip install virtualenv`

When it is finished installing, `cd` to the project folder, and run:

`virtualenv venv`

This command will create an empty virtualenv called "venv". Now you can try activating it by typing `source venv/bin/activate` and you should see (venv) on the left side of the CLI:

```shell
➜ StR_Website git:(master) ✗ source venv/bin/activate
(venv) ➜ StR_Website git:(master) ✗
```

Now we're ready to install dependencies and get everything up and running.

#### Installing Dependencies

Before we run the application, we're going to need to install the dependencies:

1. `pip install -r requirements.txt` _Note: be sure to be in venv_
2. `npm -g install bower`
3. `npm install`
4. `bower install`

If you get errors trying to install anything, save the error and let someone know.

#### Running the application (development)

Finally, we're ready to run the application:

1. Activate your virtualenv with `source venv/bin/activate`
2. Start the python application `python run.py`
3. Start the gulp server (compile CSS/JS) with just `gulp`
4. Navigate to [localhost:5000](localhost:5000) and you should have the site up and running




### Production Setup

Coming soon...

Building a Rain Gauge
---------------------

Coming soon...

Setting up a Particle Device
-----------------------------

This section will go into detail about setting up your local computer so that you can work with your Particle Electron offline. 

Smart Rain Gauge
----------------

This section will cover what components are needed to build a Smart Rain Gauge device.


### Getting Started
