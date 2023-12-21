import openai
import sys

if __name__ == '__main__':
    try:
        x = len(sys.argv)
        jointtext = ""
        for i in sys.argv:
            jointtext += i+" "
        text = jointtext
        #print(text)
        #if x==2:
            #text = sys.argv
            #print(text)

    except IOError:
        print('nie wiem co to: ' + text)


# Set up the OpenAI API client
openai.api_key = ""  #put your openai key HERE!!
 

 
 
# Set up the model and prompt
model_engine = "text-davinci-003"
#model_engine = "text-embedding-ada-002"
#prompt = "Hello, how are you today?"
prompt = text
# Generate a response
completion = openai.Completion.create(
    engine=model_engine,
    prompt=prompt,
    #max_tokens=1024,
    max_tokens=200,
    n=1,
    stop=None,
    temperature=0.5,
    #temperature=0.5,
)

response = completion.choices[0].text
print(response.strip())
