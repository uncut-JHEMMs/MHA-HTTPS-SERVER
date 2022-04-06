from locust import HttpUser , task

class HelloWorldUser(HttpUser):
    @task
    def hello_world(self):
        self.client.verify = False
        self.client.get("/hello")
    
    @task(3)
    def add_user(self):
        self.client.verify = False
        self.client.get("/addUser")