# This example requires the 'message_content' intent.
import discord
import Femboy
import re

mention_labels = [
    "zack",
    "catboy",
    "gayboy",
    "femboy",
]

async def do_commands(reply: str, message: discord.Message):
    match = re.search(r"\[KICK:\s*(.*?)\]", reply, re.IGNORECASE)
    if not match:
        return reply

    reason = match.group(1).strip() or "No reason provided"
    # Strip the [KICK: ...] tag so the chat message looks clean
    clean_reply = re.sub(
        r"\[KICK:\s*.*?\]", "", reply, flags=re.IGNORECASE
    ).strip()
    
    if not message.mentions:
        await message.channel.send("You have to mention the person that I should be kick, silly!💕🙀.")
        return
    
    target = message.mentions[0]
    if target == message.guild.me:
        if len(message.mentions) > 1: # see if there are any other mentions to kick.
            target = message.mentions[1]
        else:
            await message.channel.send("I can't kick myself, silly!.")
            return

    await target.kick(reason=reason)
        
with open("token.txt", "r") as t:
    TOKEN = t.read()

intents = discord.Intents.default()
intents.message_content = True # Requires toggle in Discord Developer Portal
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

    clean_content = message.content
    is_mentioned = False
    
    for label in mention_labels:
        if label.lower() in message.content.lower():
            is_mentioned = True
            break

    if client.user in message.mentions:
        is_mentioned = True
        clean_content = (
            clean_content.replace(f"<@{client.user.id}>", "")
            .replace(f"<@!{client.user.id}>", "")
        )

    if clean_content.startswith("!zack"):
        clean_content = clean_content.removeprefix("!zack")
        is_mentioned = True

    if not is_mentioned:
        return

    clean_content = clean_content.strip()

    print("-----------------------------------------------------------")
    print(clean_content)
    reply = Femboy.do_chat(clean_content, message.author.name)
    print(reply)

    await do_commands(reply, message)
    await message.channel.send(reply)
    

client.run(TOKEN)