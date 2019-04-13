import tornado.ioloop
import tornado.web
import datetime
import json

class Logger(object):
    def __init__(self):
        self.filename = ""
        self.fd = None
        self.new_session()

    def new_session(self):
        try:
            self.fd.close()
        except Exception:
            pass

        self.filename = datetime.datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
        self.fd = open(self.filename, "w")
        print("----- NEW SESSION STARTED", self.filename, "-----")

    def log(self, msg_type, msg):
        t = datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S")
        log_msg = "%s %s %s" % (t, msg_type, msg)
        self.fd.write(log_msg + "\n")
        print(log_msg)
        self.fd.flush()

class MainHandler(tornado.web.RequestHandler):
    def initialize(self, logger):
        self.logger = logger

    def get(self):
        self.render("template.html")

    def post(self):
        message_type = self.get_argument('message_type')
        message_text = self.get_argument('message_text')

        if message_type == "comment":
            self.logger.log(message_type, json.dumps({"text": message_text}))
        elif message_type == "temperature":
            values = json.loads(message_text)
            self.logger.log(message_type, json.dumps(values))
        elif message_type == "new_session":
            if message_text.strip() == "yes":
                self.logger.log(message_type, "bye")
                print()
                self.logger.new_session()

        self.render("template.html")


if __name__ == "__main__":
    logger = Logger()

    app = tornado.web.Application([
        (r"/", MainHandler, dict(logger=logger)),
    ])

    app.listen(8888)
    tornado.ioloop.IOLoop.current().start()

# requests.post("http://10.0.4.231:8888", data={'message_type': 'new_session', 'message_text': 'yes'})