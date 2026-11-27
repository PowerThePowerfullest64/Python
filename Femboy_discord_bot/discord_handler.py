# This example requires the 'message_content' intent.
import discord
import Femboy

with open("token.txt", "r") as t:
    TOKEN = t.read()
    
intents = discord.Intents.default()
intents.message_content = True  # Requires toggle in Discord Developer Portal
client = discord.Client(intents=intents)

@client.event
async def on_ready():
    print(f'{client.user} is connected to the following server:\n')
    for server in client.guilds:
        print(f'{server.name} (id: {server.id})')

@client.event
async def on_message(message: discord.Message):
    if message.author == client.user:
        return

    if message.content[0:5] != "!zack":
        return
    message.content = message.content[5:len(message.content)]

    print(message.content)
    reply = Femboy.do_chat(message.content, message.author.name)
    
    await message.channel.send(reply)

client.run(TOKEN)